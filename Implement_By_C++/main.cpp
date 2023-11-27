# include <iostream>
# include <string>

# include "graph.h"
# include <glm/glm.hpp>

# include <cstdlib> // For exit()
# include <stdexcept> // For std::invalid_argument

int main(int argc, char *argv[]) {

    /* Process Usr Input */

    int w = 800, h = 800;
    bool wSet = false, hSet = false;
    try{
        for (int i = 1; i<argc; i++ ) {
            std::string arg = argv[i];
            if (arg == "-w" && i + 1 < argc) {
                w = std::stoi(argv[++i]);
                wSet = true;
            }
            else if (arg == "-h" && i + 1 < argc) {
                h = std::stoi(argv[++i]);
                hSet = true;
            }
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid argument for width or height." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (wSet != hSet) {
        std::cerr << "Error: Both -w and -h must be provided together." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<Object*> scene = {
        new Sphere(glm::vec3(.75, .1, 1.), .6, glm::vec3(.8, .3, 0.)),
        new Sphere(glm::vec3(-.3, .01, .2), .3, glm::vec3(.0, .0, .9)),
        new Sphere(glm::vec3(-2.75, .1, 3.5), .6, glm::vec3(.1, .572, .184)),
        new Plane(glm::vec3(0., -.5, 0.), glm::vec3(0., 1., 0.))
    };

    rendering(
        w, h,
        scene,
        "result.png" // img save name
    );

    for (auto obj : scene) {
        delete obj;
    }

    return 0;
}
