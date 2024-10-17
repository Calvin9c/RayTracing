from .constants import *
from .fns import *
from .base_object import BaseObject
import numpy as np

class Sphere(BaseObject):
    def __init__(
            self,
            position: list, # center of the sphere
            color: list,
            reflection: float = 0.85,
            diffuse: float = 1.0,
            specular_coef: float = 0.6,
            specular_k: float = 50,
            radius: float = 0.5,
        ):
        super().__init__(position, color, reflection, diffuse, specular_coef, specular_k)
        self.radius = radius

    def intersect(self, origin: np.ndarray, direction: np.ndarray):

        OC = self.position - origin
        
        if (norm(OC) < self.radius) or (np.dot(OC, direction) < 0):
            return INF
        
        l = np.abs(np.dot(OC, direction))

        m_square = norm(OC)**2 - l**2
        q_square = self.radius**2 - m_square
        
        return (l - np.sqrt(q_square)) if q_square >= 0 else INF

    def get_normal(self, point):
        return normalize(point - self.position)

class Plane(BaseObject):
    def __init__(
            self,
            position: list,
            color: list = [1.0, 1.0, 1.0],
            reflection: float = 0.15,
            diffuse: float = 0.75,
            specular_coef: float = 0.3,
            specular_k: float = 50,
            normal: list = [0.0, 1.0, 0.0],
        ):
        super().__init__(position, color, reflection, diffuse, specular_coef, specular_k)
        self.normal = normalize(np.array(normal))

    def intersect(self, origin: np.ndarray, direction: np.ndarray):

        dn = np.dot(direction, self.normal)       
        if np.abs(dn) < EPSILON:
            return INF
        
        d = np.dot(self.position - origin, self.normal) / dn
        return d if d > 0 else INF

    def get_normal(self, *x):
        return self.normal