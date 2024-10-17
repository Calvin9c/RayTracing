# include "objects.h"
# include <glm/glm.hpp>
# include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(Objects, m) {

    py::class_<Sphere, BaseObject>(m, "Sphere")
        .def(py::init<const glm::vec3&, const glm::vec3&, float, float, float, float, float>(),
             py::arg("position"),
             py::arg("color"),
             py::arg("reflection") = 0.85f, 
             py::arg("diffuse") = 1.0f,
             py::arg("specular_coef") = 0.6f, 
             py::arg("specular_k") = 50.0f,
             py::arg("radius") = 0.5f);

    py::class_<Plane>(m, "Plane")
        .def(py::init<const glm::vec3&, const glm::vec3&, float, float, float, float, const glm::vec3&>(),
             py::arg("position"),
             py::arg("color") = glm::vec3(1.0f, 1.0f, 1.0f), 
             py::arg("reflection") = 0.15f,
             py::arg("diffuse") = 0.75f, 
             py::arg("specular_coef") = 0.3f,
             py::arg("specular_k") = 50.0f, 
             py::arg("normal") = glm::vec3(0.0f, 1.0f, 0.0f));
}