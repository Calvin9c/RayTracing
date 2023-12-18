#ifndef GRAPH_H
#define GRAPH_H

# include <cuda.h>
# include <limits>
# include <glm/glm.hpp>

using vec3 = glm::vec3;

__host__ __device__ constexpr float infinity = std::numeric_limits<float>::infinity();

/* camera */
 __device__ const vec3 camera_target = vec3(0., 0., 0.);
 __device__ const vec3 camera_pos    = vec3(0., 0.35, -1.);

/* light */
__device__ const vec3 light_point  = vec3(5., 5., -10.);
__device__ const vec3 light_color  = vec3(1., 1., 1.);
__device__ constexpr float ambient = 0.05;

enum ObjectType {
    SPHERE, PLANE
};

struct Object {
    ObjectType type;
    vec3 position;
    vec3 color;
    float reflection;
    float diffuse;
    float specular_coef;
    float specular_k = 50;
    
    // Sphere
    float radius;
    // Plane
    vec3 normal;

    Object(){}

    Object( // Sphere
        ObjectType type,
        vec3 position,
        float radius, 
        vec3 color,
        float reflection = .85,
        float diffuse = 1.,
        float specular_coef = .6,
        float specular_k = 50,
    ) : type(type), position(position), radius(radius), color(color){}

    Object( // Plane
        ObjectType type,
        vec3 position,
        vec3 normal,
        vec3 color = vec3(1., 1., 1.),
        float reflection = 0.15,
        float diffuse = .75,
        float specular_coef = .3,
        float specular_k = 50
    ) : type(type), position(position), normal(normal){}
};

__device__ float intersect(const Object& obj, const vec3& origin, const vec3& dir)

__device__ vec3 get_normal(const Object& obj, const vec3& point)

__device__ vec3 intersect_color(
    const vec3 origin, const vec3 dir, 
    const float intensity, 
    const Object* dev_scene, const int numObjects
)

__global__ void rendering_kerenl(
    const float lowerX, const float lowerY,
    const float upperX, const float upperY,
    const float stepX, const float stepY,
    const int w, const int h, 
    const Object* dev_scene, vec3* gpu_output,
    const vec3 camera_dir, const vec3 camera_right, const vec3 camera_up
)

void rendering(
    const int w, const int h,
    const Object* dev_scene, const int numObjects,
    const std::string filename
)


#endif // GRAPH_H