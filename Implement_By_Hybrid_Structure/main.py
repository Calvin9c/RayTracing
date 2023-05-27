import raytracing as rt

def main():
    plane = rt.Plane(
        rt.vec3(0.0, -0.5, 0.0), # position
        rt.vec3(0.0, 1.0, 0.0), # normal
        rt.vec3(1.0, 1.0, 1.0), # color
        0.15, # reflection
        0.75, # diffuse
        0.3, # specular_c
        50.0 # specular_k
    )

    sphere_1 = rt.Sphere(
        rt.vec3(0.75, 0.1, 1.0), # position
        0.6, # radius
        rt.vec3(0.8, 0.3, 0.0), # color
        0.85, # reflection
        1.0, # diffuse
        0.6, # specular_c
        50.0 # specular_k
    )

    sphere_2 = rt.Sphere(
        rt.create_vec3(-0.3, 0.01, 0.2), # position
        0.3, # radius
        rt.create_vec3(0.0, 0.0, 0.9), # color
        0.85, # reflection
        1.0, # diffuse
        0.6, # specular_c
        50.0 # specular_k
    )

    sphere_3 = rt.Sphere(
        rt.create_vec3(-2.75, 0.1, 3.5), # position
        0.6, # radius
        rt.create_vec3(0.1, 0.572, 0.184), # color
        0.85, # reflection
        1.0, # diffuse
        0.6, # specular_c
        50.0 # specular_k
    )

    my_scene = rt.Scene()
    my_scene.add_plane(plane)
    my_scene.add_sphere(sphere_1)
    my_scene.add_sphere(sphere_2)
    my_scene.add_sphere(sphere_3)

    rt.rendering(
        800, # w
        800, # h
        my_scene.get_scene(), # scene
        rt.create_vec3(0.0, 0.35, -1.0), # O
        rt.create_vec3(5.0, 5.0, -10.0), # light_point
        rt.create_vec3(1.0, 1.0, 1.0), # light_color
        0.05, # ambient
        "result_by_hybrid_structure.png" # filename
    )

if __name__=="__main__":
    main()