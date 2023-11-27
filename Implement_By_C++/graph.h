#ifndef GRAPH_H
#define GRAPH_H

# include <iostream>

#include <glm/glm.hpp>
#include <vector>

glm::vec3 normalize(glm::vec3 x);

class Object {
public:

    glm::vec3 position;
    glm::vec3 color;
    float reflection;
    float diffuse;
    float specular_c;
    float specular_k;

    Object(
        glm::vec3 position, 
        glm::vec3 color, 
        float reflection, 
        float diffuse, 
        float specular_c, 
        float specular_k
    );

    virtual float intersect(const glm::vec3& origin, const glm::vec3& dir) = 0;
    virtual glm::vec3 get_normal(const glm::vec3& point) = 0;
    glm::vec3 get_color();
    virtual ~Object() {}
};

class Sphere : public Object {
public:

    float radius;

    Sphere(
        glm::vec3 position, 
        float radius, 
        glm::vec3 color, 
        float reflection = .85, 
        float diffuse = 1., 
        float specular_c = .6, 
        float specular_k = 50
    );

    float intersect(const glm::vec3& origin, const glm::vec3& dir) override;

    glm::vec3 get_normal(const glm::vec3& point) override;
};

class Plane : public Object {
public:
    glm::vec3 normal;

    Plane(
        glm::vec3 position, 
        glm::vec3 normal, 
        glm::vec3 color = glm::vec3(1., 1., 1.), 
        float reflection = 0.15, 
        float diffuse = .75, 
        float specular_c = .3, 
        float specular_k = 50
    );

    float intersect(const glm::vec3& origin, const glm::vec3& dir) override;

    glm::vec3 get_normal(const glm::vec3& point) override;
};

glm::vec3 intersect_color(glm::vec3 origin, glm::vec3 dir, float intensity, std::vector<Object*> &scene, glm::vec3 light_point, glm::vec3 light_color, float ambient);

void rendering(int w, int h, std::vector<Object*> &scene, glm::vec3 O, glm::vec3 light_point, glm::vec3 light_color, float ambient, std::string filename = "test.png");

#endif // GRAPH_H