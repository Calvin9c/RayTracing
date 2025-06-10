# include "renderer.h"
# include "base_object.h"
# include "objects.h"

# include <vector>
# include <glm/glm.hpp>

# include <iostream>
# include <cstdlib>
# include <string>
# include <filesystem> 
# include <getopt.h>

namespace fs = std::filesystem;

std::string process_save_path(const std::string& result_save_path) {
    fs::path path(result_save_path);

    if (!path.has_extension()) {
        // view result_save_path as directory
        path /= "result.png"; 
    }

    // mkdir if need
    if (path.has_parent_path()) {
        fs::path directory = path.parent_path();
        if (!fs::exists(directory)) {
            fs::create_directories(directory);
        }
    }

    return path.string();
}

int main(int argc, char *argv[]){

    /* default */
    int img_h = 256;
    int img_w = 256;
    std::string result_save_path = "result.png";

    static struct option long_options[] = {
        {"img_h", required_argument, 0, 'h'},
        {"img_w", required_argument, 0, 'w'},
        {"result_save_path", required_argument, 0, 'r'},
        {0, 0, 0, 0}
    };
    int opt;
    int option_index = 0;

    /* input section */
    while ((opt = getopt_long(argc, argv, "h:w:r:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                img_h = std::atoi(optarg);
                break;
            case 'w':
                img_w = std::atoi(optarg);
                break;
            case 'r':
                result_save_path = optarg;
                break;
            default:
                std::cerr << "Unknown option" << std::endl;
                return 1;
        }
    }

    result_save_path = process_save_path(result_save_path);

    const std::vector<BaseObject*> scene = {
        new Sphere(glm::vec3(0.75, 0.1, 1.0), glm::vec3(0.8, 0.3, 0.0), 0.85, 1, 0.6, 50, 0.5),
        new Plane(glm::vec3(0.0, -0.5, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.15, 0.75, 0.3, 50, glm::vec3(0, 1, 0))
    };

    Renderer renderer = Renderer(
        img_h,
        img_w,
        scene,
        result_save_path,
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 0.35, -1.0),
        glm::vec3(5.0, 5.0, -10.0),
        glm::vec3(1.0, 1.0, 1.0),
        1e-2,
        5e-2
    );

    renderer.rendering();
    renderer.rendering_pthread();

    return 0;
}