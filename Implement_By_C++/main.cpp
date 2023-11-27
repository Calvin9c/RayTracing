# include "graph.h"
# include <glm/glm.hpp>

int main() {
    std::vector<Object*> scene = {
        new Sphere(glm::vec3(.75, .1, 1.), .6, glm::vec3(.8, .3, 0.)),
        new Sphere(glm::vec3(-.3, .01, .2), .3, glm::vec3(.0, .0, .9)),
        new Sphere(glm::vec3(-2.75, .1, 3.5), .6, glm::vec3(.1, .572, .184)),
        new Plane(glm::vec3(0., -.5, 0.), glm::vec3(0., 1., 0.))
    };

    glm::vec3 light_point = glm::vec3(5., 5., -10.);
    glm::vec3 light_color = glm::vec3(1., 1., 1.);
    float ambient = 0.05;

    int w = 800, h = 800;
    glm::vec3 O = glm::vec3(0., 0.35, -1.);

    rendering(w, h, scene, O, light_point, light_color, ambient, "result_by_cpp.png");

    for (auto obj : scene) {
        delete obj;
    }

    return 0;
}
