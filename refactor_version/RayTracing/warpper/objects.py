from .utils import *
import importlib.util
from pathlib import Path
import subprocess

def sphere(
    position: list[float], # center of the sphere
    color: list[float],
    reflection: float = 0.85,
    diffuse: float = 1.0,
    specular_coef: float = 0.6,
    specular_k: float = 50,
    radius: float = 0.5,
    impl: str = 'c'
):

    if impl == 'c':
        cpp_impl_objects = load_cpp_module('objects.so')
        SphereCPP = cpp_impl_objects.Sphere

        cpp_impl_vec = load_cpp_module('vec.so')
        VecCPP = cpp_impl_vec.vec3

        _position = VecCPP(position[0], position[1], position[2])
        _color = VecCPP(color[0], color[1], color[2])

        return SphereCPP(_position, _color, reflection, diffuse, specular_coef, specular_k, radius)

    elif impl == 'py':
        from ..py_impl.objects import Sphere as SpherePy
        return SpherePy(position, color, reflection, diffuse, specular_coef, specular_k, radius)
    else:
        raise ValueError

def plane(
    position: list[float],
    color: list[float] = [1.0, 1.0, 1.0],
    reflection: float = 0.15,
    diffuse: float = 0.75,
    specular_coef: float = 0.3,
    specular_k: float = 50,
    normal: list[float] = [0.0, 1.0, 0.0],
    impl: str = 'c'
):
    
    if impl == 'c':
        cpp_impl_objects = load_cpp_module('objects.so')
        PlaneCPP = cpp_impl_objects.Plane

        cpp_impl_vec = load_cpp_module('vec.so')
        VecCPP = cpp_impl_vec.vec3

        _position = VecCPP(position[0], position[1], position[2])
        _color = VecCPP(color[0], color[1], color[2])
        _normal = VecCPP(normal[0], normal[1], normal[2])

        return PlaneCPP(_position, _color, reflection, diffuse, specular_coef, specular_k, _normal)

    elif impl == 'py':
        from ..py_impl.objects import Plane as PlanePy
        return PlanePy(position, color, reflection, diffuse, specular_coef, specular_k, normal)
    else:
        raise ValueError