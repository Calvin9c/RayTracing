# include <glm/glm.hpp> 
# include <pybind11/pybind11.h>
# include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(vec, m) {
    py::class_<glm::vec3>(m, "vec3")
        .def(py::init<float, float, float>())
        .def_readwrite("x", &glm::vec3::x)
        .def_readwrite("y", &glm::vec3::y)
        .def_readwrite("z", &glm::vec3::z)
        .def("__repr__", [](const glm::vec3 &v) {
            return "<vec3 x=" + std::to_string(v.x) +
                   " y=" + std::to_string(v.y) +
                   " z=" + std::to_string(v.z) + ">";
        });
}