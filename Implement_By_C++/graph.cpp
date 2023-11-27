# include "graph.h"

# include <cmath>
# include <limits>
# include <opencv2/opencv.hpp>

glm::vec3 normalize(glm::vec3 x) {
    return glm::normalize(x);
}

/* class Object */
Object::Object(
    glm::vec3 position, 
    glm::vec3 color, 
    float reflection, 
    float diffuse, 
    float specular_c, 
    float specular_k
): position(position), color(color), reflection(reflection), diffuse(diffuse), specular_c(specular_c), specular_k(specular_k) {}

glm::vec3 Object::get_color() { return color; }

/* class Sphere */
Sphere::Sphere(
    glm::vec3 position, 
    float radius, 
    glm::vec3 color, 
    float reflection, 
    float diffuse, 
    float specular_c, 
    float specular_k
): Object(position, color, reflection, diffuse, specular_c, specular_k), radius(radius) {}

float Sphere::intersect(const glm::vec3& origin, const glm::vec3& dir) {
    glm::vec3 OC = position - origin;
    if (glm::length(OC) < radius || glm::dot(OC, dir) < 0)
        return std::numeric_limits<float>::infinity();
    float l = glm::length(glm::dot(OC, dir));
    float m_square = glm::length(OC) * glm::length(OC) - l * l;
    float q_square = radius * radius - m_square;
    return (q_square >= 0) ? (l - sqrt(q_square)) : std::numeric_limits<float>::infinity();
}

glm::vec3 Sphere::get_normal(const glm::vec3& point) {
    return normalize(point - position);
}

/* class Plane */
Plane::Plane(
    glm::vec3 position, 
    glm::vec3 normal, 
    glm::vec3 color, 
    float reflection, 
    float diffuse, 
    float specular_c, 
    float specular_k
): Object(position, color, reflection, diffuse, specular_c, specular_k), normal(normal) {}

float Plane::intersect(const glm::vec3& origin, const glm::vec3& dir) {
    float dn = glm::dot(dir, normal);
    if (abs(dn) < 1e-6) {
        return std::numeric_limits<float>::infinity();
    }
    float d = glm::dot(position - origin, normal) / dn;
    return d > 0 ? d : std::numeric_limits<float>::infinity();
}

glm::vec3 Plane::get_normal(const glm::vec3& point) {
    return normal;
}

/* Other */
glm::vec3 intersect_color(const glm::vec3 origin, glm::vec3 dir, float intensity, std::vector<Object*> &scene) {
    float min_distance = std::numeric_limits<float>::infinity();
    size_t obj_index=-1;
    for (size_t i = 0; i < scene.size(); ++i) {
        float current_distance = scene[i]->intersect(origin, dir);
        if (current_distance < min_distance) {
            min_distance = current_distance;
            obj_index = i;
        }
    }
    if (min_distance == std::numeric_limits<float>::infinity() || intensity < 0.01)
        return glm::vec3(0., 0., 0.);
    Object* obj = scene[obj_index];
    glm::vec3 P = origin + dir * min_distance;
    glm::vec3 color = obj->get_color();
    glm::vec3 N = obj->get_normal(P);
    glm::vec3 PL = normalize(light_point - P);
    glm::vec3 PO = normalize(origin - P);

    glm::vec3 c = ambient * color;
    std::vector<float> l;
    for (size_t i = 0; i < scene.size(); ++i) {
        if (i != obj_index)
            l.push_back(scene[i]->intersect(P + N * .0001f, PL));
    }
    if (!(l.size() > 0 && *min_element(l.begin(), l.end()) < glm::length(light_point - P))) {
        c += obj->diffuse * std::max(glm::dot(N, PL), 0.f) * color * light_color;
        c += obj->specular_c * powf(std::max(glm::dot(N, normalize(PL + PO)), 0.f), obj->specular_k) * light_color;
    }
    glm::vec3 reflect_ray = dir - 2 * glm::dot(dir, N) * N;
    c += obj->reflection * intersect_color(P + N * .0001f, reflect_ray, obj->reflection * intensity, scene);
    return glm::clamp(c, 0.f, 1.f);
}

void rendering(int w, int h, std::vector<Object*> &scene, std::string filename) {
    cv::Mat img(h, w, CV_32FC3);
    float r = float(w) / h;
    glm::vec4 S = glm::vec4(-1., -1. / r + .25, 1., 1. / r + .25);
    glm::vec3 Q = glm::vec3(0., 0., 0.);
    for (int i = 0; i < w; ++i) {
        std::cout << float(i) / w * 100 << "%" << std::endl;
        for (int j = 0; j < h; ++j) {
            Q.x = S.x + i * (S.z - S.x) / (w - 1);
            Q.y = S.y + j * (S.w - S.y) / (h - 1);
            glm::vec3 color = intersect_color(O, normalize(Q - O), 1, scene);
            img.at<cv::Vec3f>(h - j - 1, i) = cv::Vec3f(color.x, color.y, color.z);
        }
    }
    img *= 255;
    img.convertTo(img, CV_8UC3);
    cv::imwrite(filename, img);
}