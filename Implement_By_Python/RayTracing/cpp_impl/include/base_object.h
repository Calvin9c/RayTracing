# ifndef BASE_OBJECT_H
# define BASE_OBJECT_H

# include <glm/glm.hpp>

class BaseObject{

protected:
    const glm::vec3 position;
    const glm::vec3 color;
    const float reflection;
    const float diffuse;
    const float specular_coef;
    const float specular_k;

public:

    BaseObject(
        const glm::vec3& position,
        const glm::vec3& color, 
        const float reflection,
        const float diffuse, 
        const float specular_coef,
        const float specular_k
    );

    virtual ~BaseObject() = default;
    virtual float intersect(const glm::vec3& origin, const glm::vec3& direction) const = 0;
    virtual glm::vec3 get_normal(const glm::vec3& point) const = 0;

    float get_diffuse() const ;
    glm::vec3 get_color() const ;
    float get_specular_coef() const ;
    float get_specular_k() const ;
    float get_reflection() const;
};

# endif // BASE_OBJECT_H