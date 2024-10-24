import numpy as np

class BaseObject:
    def __init__(
            self,
            position: list,
            color: list,
            reflection: float,
            diffuse: float,
            specular_coef: float,
            specular_k: float
        ) -> None:

        self.position = np.array(position)
        self.color = np.array(color)
        self.reflection = reflection
        self.diffuse = diffuse
        self.specular_coef = specular_coef
        self.specular_k = specular_k

    def intersect(self):
        raise NotImplementedError
    
    def get_normal(self):
        raise NotImplementedError