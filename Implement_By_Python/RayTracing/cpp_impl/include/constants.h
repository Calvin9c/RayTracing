# ifndef CONSTANTS_H
# define CONSTANTS_H

# include <glm/glm.hpp>

constexpr float EPSILON = 1e-6;
const glm::vec3 DEFAULT_CAMERA_TARGET(0.0f, 0.0f, 0.0f);
const glm::vec3 DEFAULT_CAMERA_POS(0.0f, 0.35f, -1.0f);
const glm::vec3 DEFAULT_LIGHT_PT(5.0f, 5.0f, -10.0f);
const glm::vec3 DEFAULT_LIGHT_COLOR(1.0f, 1.0f, 1.0f);
constexpr float DEFAULT_INTENSITY_LBD = 1e-2;
constexpr float DEFAULT_AMBIENT = 5e-2;

# endif // CONSTANTS_H