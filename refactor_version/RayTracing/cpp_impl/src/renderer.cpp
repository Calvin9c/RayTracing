# include "renderer.h"
# include "base_object.h"
# include "constants.h"

# include <algorithm>
# include <cmath>
# include <vector>
# include <queue>
# include <thread>
# include <mutex>
# include <string>

# include <filesystem>
# include <iostream>
# include <fstream>

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/constants.hpp>
# include <opencv2/opencv.hpp>

Renderer::Renderer(
    const int img_h, 
    const int img_w, 
    const std::vector<BaseObject*>& scene, 
    std::string result_save_path,
    const glm::vec3& camera_target,
    const glm::vec3& camera_pos,
    const glm::vec3& light_point,
    const glm::vec3& light_color,
    const float intensity_lower_bound,
    const float ambient
): img_h(img_h), img_w(img_w), img_c(3), scene(scene), result_save_path(result_save_path),
   camera_target(camera_target), camera_pos(camera_pos),
   light_point(light_point), light_color(light_color),
   intensity_lower_bound(intensity_lower_bound), ambient(ambient)
{
    camera_dir = glm::normalize(camera_target - camera_pos);
    camera_right = glm::normalize(glm::cross(camera_dir, glm::vec3(0.0f, 1.0f, 0.0f)));
    camera_up = glm::normalize(glm::cross(camera_right, camera_dir));
}

std::tuple<float, int> Renderer::_intersect_test(const glm::vec3& origin, const glm::vec3& direction) const {

    float min_dist = INFINITY;
    int closest_obj_idx = -1;

    for (int obj_idx = 0; obj_idx < scene.size(); ++obj_idx ) {
        BaseObject* obj = scene[obj_idx];
        float dist = obj->intersect(origin, direction);

        if (dist < min_dist) {
            min_dist = dist;
            closest_obj_idx = obj_idx;
        }
    }

    return {min_dist, closest_obj_idx};
}

glm::vec3 Renderer::_half_lambert_lighting_model(
    const BaseObject& obj,
    const glm::vec3& vec_n,
    const glm::vec3& vec_pl
) const {
    glm::vec3 color = obj.get_diffuse() * std::max(glm::dot(vec_n, vec_pl), 0.0f) * obj.get_color() * light_color;
    return color;
}

glm::vec3 Renderer::_blinn_phong_model(
    const BaseObject& obj,
    const glm::vec3& vec_n,
    const glm::vec3& vec_pl,
    const glm::vec3& vec_po
) const {
    glm::vec3 color = obj.get_specular_coef() * std::pow(std::max(glm::dot(vec_n, glm::normalize(vec_pl+vec_po)), 0.0f), obj.get_specular_k()) * light_color;
    return color;
}

glm::vec3 Renderer::_shadow_test(
    const glm::vec3& pt_p,
    const int closest_obj_idx,
    const glm::vec3& vec_n,
    const glm::vec3& vec_pl,
    const glm::vec3& vec_po
) const {

    BaseObject* closest_obj = scene[closest_obj_idx];

    std::vector<float> dists;
    for (int obj_idx=0; obj_idx<scene.size(); ++obj_idx) {
        if (obj_idx != closest_obj_idx) {
            float dist = scene[obj_idx] -> intersect(pt_p+vec_n*1e-4f, vec_pl);
            dists.push_back(dist);
        }
    }

    glm::vec3 color = ambient * closest_obj -> get_color();
    if (dists.empty() || glm::length(vec_pl) <= *std::min_element(dists.begin(), dists.end())) {
        color += _half_lambert_lighting_model(*closest_obj, vec_n, vec_pl);
        color += _blinn_phong_model(*closest_obj, vec_n, vec_pl, vec_po);
    }
    return color;
}

glm::vec3 Renderer::_ray_tracing(
    const glm::vec3& origin,
    const glm::vec3& direction,
    const float intensity
) const {

    if (intensity < intensity_lower_bound) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    auto [min_dist, closest_obj_idx] = _intersect_test(origin, direction);

    if (min_dist == INFINITY) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 pt_p = origin + direction * min_dist;
    BaseObject* closest_obj = scene[closest_obj_idx];

    glm::vec3 vec_n = closest_obj -> get_normal(pt_p);
    glm::vec3 vec_pl = glm::normalize(light_point - pt_p);
    glm::vec3 vec_po = glm::normalize(origin - pt_p);

    glm::vec3 color = _shadow_test(
        pt_p,
        closest_obj_idx,
        vec_n, vec_pl, vec_po
    );

    glm::vec3 reflect = direction - 2 * glm::dot(direction, vec_n) * vec_n;
    color += closest_obj->get_reflection() * _ray_tracing(pt_p+vec_n*1e-4f, reflect, closest_obj->get_reflection()*intensity);

    return glm::clamp(color, 0.0f, 1.0f);
}

void Renderer::rendering() const {

    float aspect_ratio = static_cast<float>(img_w) / static_cast<float>(img_h);

    float left = -1.0f;
    float right = 1.0f;
    float bottom = -1.0f / aspect_ratio + 0.25f;
    float top = 1.0f / aspect_ratio + 0.25f;

    cv::Mat img = cv::Mat::zeros(img_h, img_w, CV_32FC3);
    for (int i=0; i<img_h; ++i) {
        for (int j=0; j<img_w; ++j) {
            float u = right - j * (right - left) / (img_w - 1);
            float v = top - i * (top - bottom) / (img_h - 1);

            glm::vec3 direction = glm::normalize(camera_dir + u * camera_right + v * camera_up);

            glm::vec3 color = _ray_tracing(camera_pos, direction, 1.0f);

            img.at<cv::Vec3f>(i, j)[0] = color.r;
            img.at<cv::Vec3f>(i, j)[1] = color.g;
            img.at<cv::Vec3f>(i, j)[2] = color.b;
        }
    }
    img.convertTo(img, CV_8UC3, 255.0);
    cv::imwrite(result_save_path, img);    
}

namespace {
    struct RenderTask {
        int i, j;
        glm::vec3 camera_pos, direction;
    };
    struct ThreadData {
        cv::Mat *img;
        std::queue<RenderTask> *task_que;
        std::mutex* mutex;
    };
}

void Renderer::rendering_pthread_task(void* thread_data) {
    ThreadData *data = static_cast<ThreadData*>(thread_data);
    while (true) {
        bool get_task = false;
        RenderTask task;
        {
            std::unique_lock<std::mutex> lock(*data->mutex);
            if (!data->task_que->empty()) {
                get_task = true;
                task = data->task_que->front();
                data->task_que->pop();
            }
        }
        if (!get_task) break;

        glm::vec3 color = _ray_tracing(task.camera_pos, task.direction, 1.0f);
        data->img->at<cv::Vec3f>(task.i, task.j)[0] = color.r;
        data->img->at<cv::Vec3f>(task.i, task.j)[1] = color.g;
        data->img->at<cv::Vec3f>(task.i, task.j)[2] = color.b;
    }
}

void Renderer::rendering_pthread() {

    float aspect_ratio = static_cast<float>(img_w) / static_cast<float>(img_h);

    float left = -1.0f;
    float right = 1.0f;
    float bottom = -1.0f / aspect_ratio + 0.25f;
    float top = 1.0f / aspect_ratio + 0.25f;

    cv::Mat img = cv::Mat::zeros(img_h, img_w, CV_32FC3);
    std::queue<RenderTask> task_que;

    for (int i=0; i<img_h; ++i) {
        for (int j=0; j<img_w; ++j) {
            float u = right - j * (right - left) / (img_w - 1);
            float v = top - i * (top - bottom) / (img_h - 1);
            glm::vec3 direction = 
                glm::normalize(camera_dir + u * camera_right + v * camera_up);
            task_que.push({i, j, camera_pos, direction});
        }
    }
    std::mutex mutex;
    constexpr int NUM_THREADS = 4;
    std::vector<std::thread> thread_pool;

    ThreadData thread_data{&img, &task_que, &mutex};
    for (int i=0; i<NUM_THREADS; ++i) {
        thread_pool.push_back(
            std::thread([this, &thread_data](){rendering_pthread_task(&thread_data);})
        );
    }
    for (auto& t : thread_pool) {
        t.join();
    }
    
    img.convertTo(img, CV_8UC3, 255.0);
    cv::imwrite("pthread.png", img);    
}