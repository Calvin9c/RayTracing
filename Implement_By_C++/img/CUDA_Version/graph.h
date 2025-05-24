#ifndef GRAPH_H
#define GRAPH_H

# include <cuda.h>
# include <limits>
# include <glm/glm.hpp>
# include <opencv2/opencv.hpp>

using vec3 = glm::vec3;

#ifdef __CUDACC__ // Check if the compilation is being done by NVCC
#define CUDA_DEV __device__
#define CUDA_HOST __host__
#define CUDA_GLOBAL __global__
#else
#define CUDA_DEV 
#define CUDA_HOST 
#define CUDA_GLOBAL
#endif

constexpr float infinity     = std::numeric_limits<float>::infinity();
constexpr size_t invalid_idx = std::numeric_limits<size_t>::max();
constexpr int MAX_DEPTH      = 12;

/* camera */
CUDA_DEV const vec3 camera_target = vec3(0., 0., 0.);
CUDA_DEV const vec3 camera_pos    = vec3(0., 0.35, -1.);

/* light */
CUDA_DEV const vec3 light_point  = vec3(5., 5., -10.);
CUDA_DEV const vec3 light_color  = vec3(1., 1., 1.);
constexpr float ambient          = 0.05;

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
    float specular_k;
    
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
        float reflection,
        float diffuse,
        float specular_coef,
        float specular_k
    ) : type(type), position(position), color(color), reflection(reflection), diffuse(diffuse), specular_coef(specular_coef), specular_k(specular_k), radius(radius){}

    Object( // Plane
        ObjectType type,
        vec3 position,
        vec3 normal,
        vec3 color,
        float reflection,
        float diffuse,
        float specular_coef,
        float specular_k
    ) : type(type), position(position), color(color), reflection(reflection), diffuse(diffuse), specular_coef(specular_coef), specular_k(specular_k), normal(normal){}
};

/* scene */
const Object host_scene[] = {
    Object(SPHERE, vec3(.75, .1, 1.), .6, vec3(.8, .3, 0.), .85, 1., .6, 50),
    Object(SPHERE, vec3(-.3, .01, .2), .3, vec3(.0, .0, .9), .85, 1., .6, 50),
    Object(SPHERE, vec3(-2.75, .1, 3.5), .6, vec3(.1, .572, .184), .85, 1., .6, 50),
    Object(SPHERE, vec3(.0, 1., 3.5), .6, vec3(.580, .082, .666), .85, 1., .6, 50),
    Object(PLANE,  vec3(0., -.5, 0.), vec3(0., 1., 0.), vec3(1., 1., 1.), .15, .75, .3, 50)
};
const int numObjects = sizeof(host_scene) / sizeof(Object);

/* ---------- ---------- ---------- */

CUDA_DEV float intersect(const Object& obj, const vec3& origin, const vec3& dir);

CUDA_DEV vec3 get_normal(const Object& obj, const vec3& point);

CUDA_DEV vec3 intersect_color(
    const vec3 origin, const vec3 dir, 
    const float intensity, 
    const Object* dev_scene, const int numObjects
);

CUDA_GLOBAL void rendering_kerenl(
    const float lowerX, const float lowerY,
    const float upperX, const float upperY,
    const float stepX, const float stepY,
    const int w, const int h, 
    const Object* dev_scene, vec3* gpu_output,
    const vec3 camera_dir, const vec3 camera_right, const vec3 camera_up
);

void rendering(
    const int w, const int h,
    const std::string filename
);

void rendering_balanced (
    const int w, const int h,
    const std::string filename
);

#endif // GRAPH_H