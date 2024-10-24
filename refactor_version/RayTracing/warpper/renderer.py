from .utils import *

def renderer(
    img_h: int,
    img_w: int,
    scene: list,
    result_save_path: str,
    camera_target: list = [0.0, 0.0, 0.0],
    camera_pos: list = [0.0, 0.35, -1.0],
    light_point: list = [5.0, 5.0, -10.0],
    light_color: list = [1.0, 1.0, 1.0],
    intensity_lower_bound: float = 1e-2,
    ambient: float = 5e-2,
    impl: str = 'c'
):
    
    if is_file(result_save_path):
        directory = os.path.dirname(result_save_path)
    else: # is directory
        directory = result_save_path
        if result_save_path.endswith('/'):
            result_save_path += 'result.png'
        else:
            result_save_path += '/result.png'

    mkdir(directory)

    if impl == 'c':

        cpp_impl_renderer = load_cpp_module('renderer.so')
        RendererCPP = cpp_impl_renderer.Renderer

        cpp_impl_vec = load_cpp_module('vec.so')
        VecCPP = cpp_impl_vec.vec3

        _camera_target = VecCPP(camera_target[0], camera_target[1], camera_target[2])
        _camera_pos = VecCPP(camera_pos[0], camera_pos[1], camera_pos[2])
        _light_point = VecCPP(light_point[0], light_point[1], light_point[2])
        _light_color = VecCPP(light_color[0], light_color[1], light_color[2])

        return RendererCPP(img_h, img_w, scene, result_save_path, _camera_target, _camera_pos, _light_point, _light_color, intensity_lower_bound, ambient)
    elif impl == 'py':
        from ..py_impl.renderer import Renderer as RendererPy
        return RendererPy(img_h, img_w, scene, result_save_path, camera_target, camera_pos, light_point, light_color, intensity_lower_bound, ambient)
    else:
        raise ValueError