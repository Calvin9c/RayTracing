from .constants import *
from .fns import *
from .objects import BaseObject

from tqdm import tqdm
import numpy as np
import matplotlib.pyplot as plt

class Renderer:

    def __init__(
            self,
            img_h: int,
            img_w: int,
            scene: list,
            result_save_path: str,
            camera_target: list[float],
            camera_pos: list[float],
            light_point: list[float],
            light_color: list[float],
            intensity_lower_bound: float,
            ambient: float,
        ):
        self.img_h, self.img_w, self.img_c = img_h, img_w, 3
        self.scene = scene.copy()

        self.result_save_path = result_save_path

        # camera related variables
        self.camera_target = camera_target if isinstance(camera_target, np.ndarray) \
                                           else np.array(camera_target)
        self.camera_pos = camera_pos if isinstance(camera_pos, np.ndarray) \
                                     else np.array(camera_pos)
        self.camera_dir = normalize(self.camera_target - self.camera_pos)
        self.camera_right = \
            normalize(np.cross(self.camera_dir, np.array([0.0, 1.0, 0.0])))
        self.camera_up = \
            normalize(np.cross(self.camera_right, self.camera_dir))

        # light related variables
        self.light_point = light_point if isinstance(light_point, np.ndarray) \
                                       else np.array(light_point)
        self.light_color = light_color if isinstance(light_color, np.ndarray) \
                                       else np.array(light_color)
        
        self.intensity_lower_bound = intensity_lower_bound
        self.ambient = ambient

    def _intersect_test(
            self,
            origin: np.ndarray,
            direction: np.ndarray
        ):

        min_dist, closest_obj_idx = INF, None
        for obj_idx, obj in enumerate(self.scene):
            dist = obj.intersect(origin, direction)
            if dist < min_dist:
                min_dist, closest_obj_idx = dist, obj_idx

        return min_dist, closest_obj_idx

    def _half_lambert_lighting_model(
            self,
            obj: BaseObject,
            vec_n: np.ndarray, # normal vector
            vec_pl: np.ndarray # vector from p to light_point with a length of 1
        ) -> np.ndarray:

        color = obj.diffuse * max(np.dot(vec_n, vec_pl), 0.0) * obj.color * self.light_color

        return color

    def _blinn_phong_model(
            self,
            obj: BaseObject,
            vec_n: np.ndarray, # normal vector
            vec_pl: np.ndarray, # vector from p to light_point with a length of 1
            vec_po: np.ndarray # vector from p to origin with a length of 1
        ) -> np.ndarray:

        color = obj.specular_coef * max(np.dot(vec_n, normalize(vec_pl+vec_po)), 0.0) ** obj.specular_k * self.light_color
        
        return color

    def _shadow_test(
            self,
            pt_p: np.ndarray,
            closest_obj_idx: int,
            vec_n: np.ndarray, # normal vector
            vec_pl: np.ndarray, # vector from p to light_point with a length of 1
            vec_po: np.ndarray, # vector from p to origin with a length of 1
        ) -> np.ndarray:

        closest_obj = self.scene[closest_obj_idx]

        dists = [self.scene[obj_idx].intersect(pt_p+vec_n*1e-4, vec_pl) for obj_idx in range(len(self.scene)) \
                 if obj_idx != closest_obj_idx]

        color = self.ambient * closest_obj.color
        if 0 == len(dists) or norm(vec_pl) <= min(dists):
            color += self._half_lambert_lighting_model(closest_obj, vec_n, vec_pl)
            color += self._blinn_phong_model(closest_obj, vec_n, vec_pl, vec_po)

        return color

    def _ray_tracing(
            self,
            origin: np.ndarray,
            direction: np.ndarray,
            intensity: float
        ) -> np.ndarray:

        # find the closest object
        if intensity < self.intensity_lower_bound:
            return np.zeros(3)

        min_dist, closest_obj_idx = self._intersect_test(origin, direction)

        if min_dist == INF:
            return np.zeros(3)

        pt_p = origin + direction * min_dist
        closest_obj = self.scene[closest_obj_idx]

        vec_n = closest_obj.get_normal(pt_p)
        vec_pl = normalize(self.light_point - pt_p)
        vec_po = normalize(origin - pt_p)

        color = \
            self._shadow_test(pt_p, closest_obj_idx, vec_n, vec_pl, vec_po)

        reflect = direction - 2 * np.dot(direction, vec_n) * vec_n
        color += \
            closest_obj.reflection * self._ray_tracing(pt_p+vec_n*1e-4, reflect, closest_obj.reflection*intensity)

        return np.clip(color, 0, 1)

    def rendering(self):
        
        aspect_ratio = self.img_w / self.img_h
        view_frustum = \
            [-1.0, -1.0/aspect_ratio+0.25, 1.0, 1.0/aspect_ratio+0.25]
        
        img = np.zeros((self.img_h, self.img_w, self.img_c))
        for i in tqdm(range(self.img_h)):
            for j in range(self.img_w):
                u = view_frustum[2] - j*(view_frustum[2]-view_frustum[0])/(self.img_w-1)
                v = view_frustum[3] - i*(view_frustum[3]-view_frustum[1])/(self.img_h-1)
                direction = normalize(self.camera_dir + u*self.camera_right + v*self.camera_up)
                img[i, j] = self._ray_tracing(self.camera_pos, direction, 1)

        plt.imsave(self.result_save_path, img)