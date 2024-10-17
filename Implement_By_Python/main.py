from RayTracing import Renderer, Sphere, Plane

def main():

    scene = [
        Sphere(position=[0.75, 0.1, 1.0], radius=0.6, color=[0.8, 0.3, 0.0]),
        Sphere(position=[-0.3, 0.01, 0.2], radius=0.3, color=[0.0, 0.0, 0.9]),
        Sphere(position=[-2.75, 0.1, 3.5], radius=0.6, color=[0.1, 0.572, 0.184]),
        Plane(position=[0.0, -0.5, 0.0])
    ]

    renderer = Renderer(512, 512, scene, 'result.png')
    renderer.rendering()

if __name__ == '__main__':
    main()