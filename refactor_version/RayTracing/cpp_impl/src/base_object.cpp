# include "base_object.h"
# include <glm/glm.hpp>

BaseObject::BaseObject(
    const glm::vec3& position,
    const glm::vec3& color, 
    const float reflection,
    const float diffuse, 
    const float specular_coef,
    const float specular_k
) : position(position), color(color), reflection(reflection), diffuse(diffuse), 
    specular_coef(specular_coef), specular_k(specular_k) {}

float BaseObject::get_diffuse() const {
    return diffuse;
}

glm::vec3 BaseObject::get_color() const {
    return color;
}

float BaseObject::get_specular_coef() const {
    return specular_coef;
}

float BaseObject::get_specular_k() const {
    return specular_k;
}

float BaseObject::get_reflection() const {
    return reflection;
}