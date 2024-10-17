from .utils import *

def renderer(
    img_h: int,
    img_w: int,
    scene: list,
    result_save_path: str,
    camera_target: list,
    camera_pos: list,
    light_point: list,
    light_color: list,
    intensity_lower_bound: float,
    ambient: float
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

    # TODO

    return