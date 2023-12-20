# include <iostream>

# include "graph.h"
# include <chrono> 

# include <cstdlib> // For exit()
# include <stdexcept> // For std::invalid_argument

int main(int argc, char *argv[]) {

    /* Process Usr Input */

    int w = 3840, h = 2160;
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
        std::exit(EXIT_FAILURE);
    }

    const std::vector<Object*> scene = {
        new Sphere(vec3(.75, .1, 1.), .6, vec3(.8, .3, 0.)),
        new Sphere(vec3(-.3, .01, .2), .3, vec3(.0, .0, .9)),
        new Sphere(vec3(-2.75, .1, 3.5), .6, vec3(.1, .572, .184)),
        new Sphere(vec3(.0, 1., 3.5), .6, vec3(.580, .082, .666)),
        new Plane(vec3(0., -.5, 0.), vec3(0., 1., 0.))
    };
    auto start_time = std::chrono::high_resolution_clock::now();
    rendering(
        w, h,
        scene,
        "result.png" // img save name
    );
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Rendering completed in " << duration.count() << " milliseconds." << std::endl;

    for (auto obj : scene) { delete obj; }

    return 0;
}
