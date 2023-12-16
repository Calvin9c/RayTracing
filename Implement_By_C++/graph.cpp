# include "graph.h"

# include <cmath>
# include <opencv2/opencv.hpp>

/* class Object */
Object::Object(
    vec3 position, 
    vec3 color, 
    float reflection, 
    float diffuse, 
    float specular_c, 
    float specular_k
): position(position), color(color), reflection(reflection), diffuse(diffuse), specular_c(specular_c), specular_k(specular_k) {}

vec3 Object::get_color() { return color; }

/* class Sphere */
Sphere::Sphere(
    vec3 position, 
    float radius, 
    vec3 color, 
    float reflection, 
    float diffuse, 
    float specular_c, 
    float specular_k
): Object(position, color, reflection, diffuse, specular_c, specular_k), radius(radius) {}

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
    vec3 position, 
    vec3 normal, 
    vec3 color, 
    float reflection, 
    float diffuse, 
    float specular_c, 
    float specular_k
): Object(position, color, reflection, diffuse, specular_c, specular_k), normal(normal) {}

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

    std::vector<float> l;
    for (size_t i = 0; i < scene.size(); ++i) {
        if (i != obj_index)
            l.push_back(scene[i]->intersect(P + N * .0001f, PL));
    }
    if (!(l.size() > 0 && *min_element(l.begin(), l.end()) < glm::length(PL) )) {
        c += obj->diffuse * std::max(glm::dot(N, PL), 0.f) * color * light_color;
        c += obj->specular_c * powf(std::max(glm::dot(N, glm::normalize(PL + PO)), 0.f), obj->specular_k) * light_color;
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

    cv::Mat img(h, w, CV_32FC3);
    
    // aspect ratio
    const float r = float(w) / h;
    
    // view frustum
    glm::vec4 S = glm::vec4(-1., -1. / r + .25, 1., 1. / r + .25); 
    
    vec3 camera_dir = vec3(0., 0., 0.); // point position that camera point to.
    
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w ; ++j) {
            camera_dir.x = S.x + j * (S.z - S.x) / (w - 1);
            camera_dir.y = S.w - i * (S.w - S.y) / (h - 1);
            vec3 color = intersect_color(camera_pos, glm::normalize(camera_dir - camera_pos), 1, scene);
            img.at<cv::Vec3f>(i, j) = cv::Vec3f(color.x, color.y, color.z);
        }
    }

    img *= 255;
    
    img.convertTo(img, CV_8UC3);
    
    cv::imwrite(filename, img);
}