#ifndef GRAPH_H
#define GRAPH_H

# include <string>
# include <limits>
# include <glm/glm.hpp>
# include <vector>

# define PI 3.1415926

using vec3 = glm::vec3;

constexpr float infinity = std::numeric_limits<float>::infinity();
const vec3 light_point   = vec3(5., 5., -10.);
const vec3 light_color   = vec3(1., 1., 1.);
constexpr float ambient  = 0.05;

class Object {
public:

    const vec3 position;
    const vec3 color;
    const float reflection;
    const float diffuse;
    const float specular_coef;
    const float specular_k;

    Object(
        vec3 position, 
        vec3 color, 
        float reflection, 
        float diffuse, 
        float specular_coef, 
        float specular_k
    );

    virtual float intersect(const vec3& origin, const vec3& dir) = 0;
    virtual vec3 get_normal(const vec3& point) = 0;
    vec3 get_color();
    virtual ~Object() {}
};

class Sphere : public Object {
public:

    const float radius;

    Sphere(
        vec3 position, 
        float radius, 
        vec3 color, 
        float reflection    = .85, 
        float diffuse       = 1., 
        float specular_coef = .6, 
        float specular_k    = 50
    );

    float intersect(const vec3& origin, const vec3& dir) override;

    vec3 get_normal(const vec3& point) override;
};

class Plane : public Object {
public:

    const vec3 normal;

    Plane(
        vec3 position, 
        vec3 normal, 
        vec3 color          = vec3(1., 1., 1.), 
        float reflection    = 0.15, 
        float diffuse       = .75, 
        float specular_coef = .3, 
        float specular_k    = 50
    );

    float intersect(const vec3& origin, const vec3& dir) override;

    vec3 get_normal(const vec3& point) override;
};

vec3 intersect_color(
    const vec3 origin, 
    const vec3 dir, 
    const float intensity, 
    const std::vector<Object*> &scene
);

void rendering(
    const int w, const int h,
    const std::vector<Object*> &scene,
    const std::string filename
);

#endif // GRAPH_H