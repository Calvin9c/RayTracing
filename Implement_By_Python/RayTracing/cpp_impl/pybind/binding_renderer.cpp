# include "objects.h"
# include "renderer.h"
# include "constants.h"
# include <string>
# include <vector>
# include <glm/glm.hpp>
# include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(Renderer, m) {
    py::class_<Renderer>(m, "Renderer")
        .def(py::init<const int, const int, const std::vector<BaseObject*>&, std::string,
             const glm::vec3&, const glm::vec3&, const glm::vec3&, const glm::vec3&, const float, const float>(),
             py::arg("img_h"),
             py::arg("img_w"),
             py::arg("scene"),
             py::arg("result_save_path"),
             py::arg("camera_target") = DEFAULT_CAMERA_TARGET,
             py::arg("camera_pos") = DEFAULT_CAMERA_POS,
             py::arg("light_point") = DEFAULT_LIGHT_PT,
             py::arg("light_color") = DEFAULT_LIGHT_COLOR,
             py::arg("intensity_lower_bound") = DEFAULT_INTENSITY_LBD,
             py::arg("ambient") = DEFAULT_AMBIENT)
        .def("rendering", &Renderer::rendering);
}