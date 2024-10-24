from RayTracing import renderer, sphere, plane
import argparse

def main(
        img_h: int,
        img_w: int,
        result_save_path: str,
        impl: str
    ):

    assert impl in ['c', 'py']

    scene = [
        sphere(position=[0.75, 0.1, 1.0], radius=0.6, color=[0.8, 0.3, 0.0], impl=impl),
        sphere(position=[-0.3, 0.01, 0.2], radius=0.3, color=[0.0, 0.0, 0.9], impl=impl),
        sphere(position=[-2.75, 0.1, 3.5], radius=0.6, color=[0.1, 0.572, 0.184], impl=impl),
        plane(position=[0.0, -0.5, 0.0], impl=impl)
    ]

    r = renderer(img_h, img_w, scene, result_save_path, impl=impl)
    r.rendering()

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description="RayTracing Renderer")

    parser.add_argument('--img_h', type=int, default=512, help='Image height (default: 512)')
    parser.add_argument('--img_w', type=int, default=512, help='Image width (default: 512)')
    parser.add_argument('--result_save_path', type=str, default='result.png', help='Path to save the result image (default: result.png)')
    parser.add_argument('--impl', type=str, default='c', choices=['c', 'py'], help="Implementation to use: 'c' for C++ or 'py' for Python (default: 'c')")

    args = parser.parse_args()

    main(
        img_h=args.img_h,
        img_w=args.img_w,
        result_save_path=args.result_save_path,
        impl=args.impl
    )