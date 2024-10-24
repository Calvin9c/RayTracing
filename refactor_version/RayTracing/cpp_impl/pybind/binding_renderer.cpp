# include "objects.h"
# include "renderer.h"
# include "constants.h"
# include <string>
# include <vector>
# include <glm/glm.hpp>
# include <pybind11/pybind11.h>
# include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(renderer, m) {
    py::class_<Renderer>(m, "Renderer")
        .def(py::init<const int, const int, const std::vector<BaseObject*>&, std::string,
             const glm::vec3&, const glm::vec3&, const glm::vec3&, const glm::vec3&, const float, const float>(),
             py::arg("img_h"),
             py::arg("img_w"),
             py::arg("scene"),
             py::arg("result_save_path"),
             py::arg("camera_target"),
             py::arg("camera_pos"),
             py::arg("light_point"),
             py::arg("light_color"),
             py::arg("intensity_lower_bound"),
             py::arg("ambient"))
        .def("rendering", &Renderer::rendering);
}