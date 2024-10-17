#include "constants.h"
#include "objects.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>

/* ========== ========== ========== */
/* Sphere */
/* ========== ========== ========== */

Sphere::Sphere(
    const glm::vec3& position,
    const glm::vec3& color,
    const float reflection, 
    const float diffuse,
    const float specular_coef,
    const float specular_k,
    const float radius
): BaseObject(position, color, reflection, diffuse, specular_coef, specular_k), radius(radius) {}

float Sphere::intersect(const glm::vec3& origin, const glm::vec3& direction) const {
    glm::vec3 OC = position - origin;

    if (glm::length(OC) < radius || glm::dot(OC, direction) < 0){
        return INFINITY;
    }

    float l = std::abs(glm::dot(OC, direction));

    float m_square = glm::length(OC)*glm::length(OC) - l*l;
    float q_square = radius*radius - m_square;

    float dist = 0<=q_square ? l-std::sqrt(q_square) : INFINITY;
    return dist;
}

glm::vec3 Sphere::get_normal(const glm::vec3& point) const {
    return glm::normalize(point - position);
}

/* ========== ========== ========== */
/* Plane */
/* ========== ========== ========== */

Plane::Plane(
    const glm::vec3& position,
    const glm::vec3& color,
    float reflection,
    float diffuse,
    float specular_coef,
    float specular_k,
    const glm::vec3& normal
): BaseObject(position, color, reflection, diffuse, specular_coef, specular_k), normal(glm::normalize(normal)) {}

float Plane::intersect(const glm::vec3& origin, const glm::vec3& direction) const {
    float dn = glm::dot(direction, normal);
    if (std::abs(dn) < EPSILON) {
        return INFINITY;
    }

    float d = glm::dot(position-origin, normal) / dn;

    float dist = 0 < d ? d : INFINITY;

    return dist;
}

glm::vec3 Plane::get_normal(const glm::vec3& /*points*/) const {
    return normal;
}