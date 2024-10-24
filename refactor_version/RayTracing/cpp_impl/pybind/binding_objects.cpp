# include "objects.h"
# include <glm/glm.hpp>
# include <pybind11/pybind11.h>
# include <pybind11/stl.h>
# include <memory>

namespace py = pybind11;

PYBIND11_MODULE(objects, m) {

    py::class_<BaseObject, std::shared_ptr<BaseObject>>(m, "BaseObject");

    py::class_<Sphere, std::shared_ptr<Sphere>, BaseObject>(m, "Sphere")
        .def(py::init<const glm::vec3&, const glm::vec3&, float, float, float, float, float>(),
             py::arg("position"),
             py::arg("color"),
             py::arg("reflection"), 
             py::arg("diffuse"),
             py::arg("specular_coef"), 
             py::arg("specular_k"),
             py::arg("radius"));

    py::class_<Plane, std::shared_ptr<Plane>, BaseObject>(m, "Plane")
        .def(py::init<const glm::vec3&, const glm::vec3&, float, float, float, float, const glm::vec3&>(),
             py::arg("position"),
             py::arg("color"), 
             py::arg("reflection"),
             py::arg("diffuse"), 
             py::arg("specular_coef"),
             py::arg("specular_k"), 
             py::arg("normal"));
}