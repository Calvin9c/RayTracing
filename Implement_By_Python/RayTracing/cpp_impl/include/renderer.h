# ifndef RENDERER_H
# define RENDERER_H

# include "base_object.h"
# include "constants.h"
# include <string>
# include <vector>
# include <tuple>

class Renderer{

protected:
    const int img_h, img_w, img_c;
    const std::vector<BaseObject*> scene;
    std::string result_save_path;
    
    // camera related variables
    glm::vec3 camera_target;
    glm::vec3 camera_pos;
    glm::vec3 camera_dir;
    glm::vec3 camera_right;
    glm::vec3 camera_up;

    // light related variables
    glm::vec3 light_point;
    glm::vec3 light_color;
    const float intensity_lower_bound;
    const float ambient;

public:

    Renderer(
        const int img_h, 
        const int img_w, 
        const std::vector<BaseObject*>& scene, 
        std::string result_save_path,
        const glm::vec3& camera_target,
        const glm::vec3& camera_pos,
        const glm::vec3& light_point,
        const glm::vec3& light_color,
        const float intensity_lower_bound,
        const float ambient
    );

    std::tuple<float, int> _intersect_test(const glm::vec3& origin, const glm::vec3& direction) const ;

    glm::vec3 _half_lambert_lighting_model(
        const BaseObject& obj,
        const glm::vec3& vec_n,
        const glm::vec3& vec_pl
    ) const ;

    glm::vec3 _blinn_phong_model(
        const BaseObject& obj,
        const glm::vec3& vec_n,
        const glm::vec3& vec_pl,
        const glm::vec3& vec_po
    ) const ;

    glm::vec3 _shadow_test(
        const glm::vec3& pt_p,
        const int closest_obj_idx,
        const glm::vec3& vec_n,
        const glm::vec3& vec_pl,
        const glm::vec3& vec_po
    ) const ;

    glm::vec3 _ray_tracing(
        const glm::vec3& origin,
        const glm::vec3& direction,
        const float intensity
    ) const ;

    void rendering() const ;

};

# endif // RENDERER_H