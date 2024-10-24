# include "renderer.h"
# include "base_object.h"
# include "objects.h"

# include <vector>
# include <glm/glm.hpp>

int main(){

    const std::vector<BaseObject*> scene = {
        new Sphere(glm::vec3(0.75, 0.1, 1.0), glm::vec3(0.8, 0.3, 0.0), 0.85, 1, 0.6, 50, 0.5),
        new Plane(glm::vec3(0.0, -0.5, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.15, 0.75, 0.3, 50, glm::vec3(0, 1, 0))
    };

    Renderer renderer = Renderer(
        256,
        256,
        scene,
        "result.png",
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 0.35, -1.0),
        glm::vec3(5.0, 5.0, -10.0),
        glm::vec3(1.0, 1.0, 1.0),
        1e-2,
        5e-2
    );

    renderer.rendering();

    return 0;
}