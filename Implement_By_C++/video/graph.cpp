# include "graph.h"
# include "video_cfg.h"

# include <cmath>
# include <opencv2/opencv.hpp>

/* class Object */
Object::Object(
    vec3 position, 
    vec3 color, 
    float reflection, 
    float diffuse, 
    float specular_coef, 
    float specular_k
): position(position), color(color), reflection(reflection), diffuse(diffuse), specular_coef(specular_coef), specular_k(specular_k) {}

vec3 Object::get_color() { return color; }

/* class Sphere */
Sphere::Sphere(
    vec3 position, float radius, 
    vec3 color, 
    float reflection, 
    float diffuse, 
    float specular_coef, 
    float specular_k
): Object(position, color, reflection, diffuse, specular_coef, specular_k), radius(radius) {}

float Sphere::intersect(const vec3& origin, const vec3& dir) {

    const vec3 OC = position - origin;

    if (glm::length(OC) < radius || glm::dot(OC, dir) < 0) return infinity;

    float l        = glm::length(glm::dot(OC, dir));
    float m_square = glm::length(OC) * glm::length(OC) - l * l;
    float q_square = radius * radius - m_square;

    return (q_square >= 0) ? (l - sqrt(q_square)) : infinity;

}

vec3 Sphere::get_normal(const vec3& point) { return glm::normalize(point - position); }

/* class Plane */
Plane::Plane(
    vec3 position, vec3 normal, 
    vec3 color, 
    float reflection, 
    float diffuse, 
    float specular_coef, 
    float specular_k
): Object(position, color, reflection, diffuse, specular_coef, specular_k), normal(normal) {}

float Plane::intersect(const vec3& origin, const vec3& dir) {

    float dn = glm::dot(dir, normal);
    
    if (abs(dn) < 1e-6) { return infinity; }
    
    float d = glm::dot(position - origin, normal) / dn;
    
    return d > 0 ? d : infinity;
}

vec3 Plane::get_normal(const vec3& point) { return normal; }

/* Other */
vec3 intersect_color(
    const vec3 origin,
    const vec3 dir,
    const float intensity,
    const std::vector<Object*> &scene
) {

    float min_distance = infinity;

    size_t obj_index=-1;
    for (size_t i = 0; i < scene.size(); ++i) {
        float current_distance = scene[i]->intersect(origin, dir);
        if (current_distance < min_distance) {
            min_distance = current_distance;
            obj_index = i;
        }
    }

    if (min_distance == infinity || intensity < 0.01) return vec3(0., 0., 0.);
    
    Object* obj   = scene[obj_index];
    const vec3 P  = origin + dir * min_distance;
    const vec3 PL = glm::normalize(light_point - P);
    const vec3 PO = glm::normalize(origin - P);
    const vec3 N  = obj->get_normal(P);

    const vec3 color = obj->get_color();
    vec3 c = ambient * color;

    /*shadow test*/
    std::vector<float> l;
    for (size_t i = 0; i < scene.size(); ++i) {
        if (i != obj_index)
            l.push_back(scene[i]->intersect(P + N * .0001f, PL));
    }

    if (!(l.size() > 0 && *min_element(l.begin(), l.end()) < glm::length(PL) )) {
        c += obj->diffuse * std::max(glm::dot(N, PL), 0.f) * color * light_color;
        c += obj->specular_coef * powf(std::max(glm::dot(N, glm::normalize(PL + PO)), 0.f), obj->specular_k) * light_color;
    }
    vec3 reflect_ray = dir - 2 * glm::dot(dir, N) * N;
    c += obj->reflection * intersect_color(P + N * .0001f, reflect_ray, obj->reflection * intensity, scene);
    return glm::clamp(c, 0.f, 1.f);
}

void rendering(
    const int w, const int h, 
    const std::vector<Object*> &scene,
    const std::string filename
) {

    cv::VideoWriter v_wrt;
    v_wrt.open(
        filename, 
        cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), //fourcc
        fps,
        cv::Size(w, h)
    );
    if (!v_wrt.isOpened()) {
        throw std::runtime_error("Error opening video writer");
    }    

    const double radius      = 4;                                               // camera rotate radius
    const vec3 camera_target = vec3(0., 0., 0.);
    const float r            = float(w) / h;                                    // aspect ratio
    const glm::vec4 S        = glm::vec4(-1., -1. / r + .25, 1., 1. / r + .25); // view frustum
    
    constexpr int total_frame = (int)video_length * fps;
    for( int frame_idx = 0; frame_idx < total_frame ; ++frame_idx ){

        cv::Mat frame(h, w, CV_32FC3);

        const double theta      = (double)frame_idx * ( 2*PI / (video_length * fps) );
        const vec3 camera_pos   = vec3(camera_target.x + radius * cos( theta ), 0.35, camera_target.z + radius * sin(theta));
        const vec3 camera_dir   = glm::normalize(camera_target - camera_pos);
        const vec3 camera_right = glm::normalize(glm::cross(camera_dir, vec3(0, 1, 0)));
        const vec3 camera_up    = glm::normalize(glm::cross(camera_right, camera_dir));        
        
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w ; ++j) {
                float u = S.x + j * (S.z - S.x) / (w - 1);
                float v = S.w - i * (S.w - S.y) / (h - 1);

                vec3 direction = glm::normalize(camera_dir + u * camera_right * S.z + v * camera_up * S.w);

                vec3 color = intersect_color(camera_pos, direction, 1, scene);
                frame.at<cv::Vec3f>(i, j) = cv::Vec3f(color.x, color.y, color.z);
            }
        }

        frame *= 255;
        frame.convertTo(frame, CV_8UC3);
        v_wrt.write(frame);
    }
    v_wrt.release();

}