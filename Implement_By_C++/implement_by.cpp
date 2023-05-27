#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>

glm::vec3 normalize(glm::vec3 x) {
    return glm::normalize(x);
}

class Object {
public:
    glm::vec3 position;
    glm::vec3 color;
    float reflection;
    float diffuse;
    float specular_c;
    float specular_k;

    Object(glm::vec3 position, glm::vec3 color, float reflection, float diffuse, float specular_c, float specular_k) :
        position(position), color(color), reflection(reflection), diffuse(diffuse), specular_c(specular_c), specular_k(specular_k) {}

    virtual float intersect(const glm::vec3& origin, const glm::vec3& dir) = 0;
    virtual glm::vec3 get_normal(const glm::vec3& point) = 0;
    glm::vec3 get_color() { return color; }
    virtual ~Object() {}
};

class Sphere : public Object {
public:
    float radius;

    Sphere(glm::vec3 position, float radius, glm::vec3 color, float reflection = .85, float diffuse = 1., float specular_c = .6, float specular_k = 50) :
        Object(position, color, reflection, diffuse, specular_c, specular_k), radius(radius) {}

    float intersect(const glm::vec3& origin, const glm::vec3& dir) override {
        glm::vec3 OC = position - origin;
        if (glm::length(OC) < radius || glm::dot(OC, dir) < 0)
            return std::numeric_limits<float>::infinity();
        float l = glm::length(glm::dot(OC, dir));
        float m_square = glm::length(OC) * glm::length(OC) - l * l;
        float q_square = radius * radius - m_square;
        return (q_square >= 0) ? (l - sqrt(q_square)) : std::numeric_limits<float>::infinity();
    }

    glm::vec3 get_normal(const glm::vec3& point) override {
        return normalize(point - position);
    }
};

class Plane : public Object {
public:
    glm::vec3 normal;

    Plane(glm::vec3 position, glm::vec3 normal, glm::vec3 color = glm::vec3(1., 1., 1.), float reflection = 0.15, float diffuse = .75, float specular_c = .3, float specular_k = 50) :
        Object(position, color, reflection, diffuse, specular_c, specular_k), normal(normal) {}

    float intersect(const glm::vec3& origin, const glm::vec3& dir) override {
        float dn = glm::dot(dir, normal);
        if (abs(dn) < 1e-6) {
            return std::numeric_limits<float>::infinity();
        }
        float d = glm::dot(position - origin, normal) / dn;
        return d > 0 ? d : std::numeric_limits<float>::infinity();
    }

    glm::vec3 get_normal(const glm::vec3& point) override {
        return normal;
    }
};

glm::vec3 intersect_color(glm::vec3 origin, glm::vec3 dir, float intensity, std::vector<Object*> &scene, glm::vec3 light_point, glm::vec3 light_color, float ambient) {
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
    c += obj->reflection * intersect_color(P + N * .0001f, reflect_ray, obj->reflection * intensity, scene, light_point, light_color, ambient);
    return glm::clamp(c, 0.f, 1.f);
}

void rendering(int w, int h, std::vector<Object*> &scene, glm::vec3 O, glm::vec3 light_point, glm::vec3 light_color, float ambient, std::string filename = "test.png") {
    cv::Mat img(h, w, CV_32FC3);
    float r = float(w) / h;
    glm::vec4 S = glm::vec4(-1., -1. / r + .25, 1., 1. / r + .25);
    glm::vec3 Q = glm::vec3(0., 0., 0.);
    for (int i = 0; i < w; ++i) {
        std::cout << float(i) / w * 100 << "%" << std::endl;
        for (int j = 0; j < h; ++j) {
            Q.x = S.x + i * (S.z - S.x) / (w - 1);
            Q.y = S.y + j * (S.w - S.y) / (h - 1);
            glm::vec3 color = intersect_color(O, normalize(Q - O), 1, scene, light_point, light_color, ambient);
            img.at<cv::Vec3f>(h - j - 1, i) = cv::Vec3f(color.x, color.y, color.z);
        }
    }
    img *= 255;
    img.convertTo(img, CV_8UC3);
    cv::imwrite(filename, img);
}

int main() {
    std::vector<Object*> scene = {
        new Sphere(glm::vec3(.75, .1, 1.), .6, glm::vec3(.8, .3, 0.)),
        new Sphere(glm::vec3(-.3, .01, .2), .3, glm::vec3(.0, .0, .9)),
        new Sphere(glm::vec3(-2.75, .1, 3.5), .6, glm::vec3(.1, .572, .184)),
        new Plane(glm::vec3(0., -.5, 0.), glm::vec3(0., 1., 0.))
    };

    glm::vec3 light_point = glm::vec3(5., 5., -10.);
    glm::vec3 light_color = glm::vec3(1., 1., 1.);
    float ambient = 0.05;

    int w = 800, h = 800;
    glm::vec3 O = glm::vec3(0., 0.35, -1.);

    rendering(w, h, scene, O, light_point, light_color, ambient, "result_by_cpp.png");

    for (auto obj : scene) {
        delete obj;
    }

    return 0;
}
