# ifndef OBJECTS_H
# define OBJECTS_H

# include "base_object.h"

class Sphere : public BaseObject {
protected:
    const float radius;

public:
    Sphere(
        const glm::vec3& position,
        const glm::vec3& color,
        const float reflection, 
        const float diffuse,
        const float specular_coef,
        const float specular_k,
        const float radius
    );

    float intersect(const glm::vec3& origin, const glm::vec3& direction) const override;

    glm::vec3 get_normal(const glm::vec3& point) const override;

};

class Plane : public BaseObject{
protected:
    const glm::vec3 normal;
public:
    Plane(
        const glm::vec3& position,
        const glm::vec3& color,
        const float reflection,
        const float diffuse,
        const float specular_coef,
        const float specular_k,
        const glm::vec3& normal
    );

    float intersect(const glm::vec3& origin, const glm::vec3& direction) const override;

    glm::vec3 get_normal(const glm::vec3& /*points*/) const override;
};

# endif // OBJECTS_H