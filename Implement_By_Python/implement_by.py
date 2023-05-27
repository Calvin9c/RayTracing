import numpy as np
import matplotlib.pyplot as plt

def normalize(x):
    return x / np.linalg.norm(x)

class Object:
    def __init__(self, position, color, reflection, diffuse, specular_c, specular_k):
        self.position = np.array(position)
        self.color = np.array(color)
        self.reflection = reflection
        self.diffuse = diffuse
        self.specular_c = specular_c
        self.specular_k = specular_k

    def get_color(self):
        return self.color

class Sphere(Object):
    def __init__(self, position, radius, color, reflection=.85, diffuse=1., specular_c=.6, specular_k=50):
        super().__init__(position, color, reflection, diffuse, specular_c, specular_k)
        self.radius = np.array(radius)

    def intersect(self, origin, dir):

        OC = self.position - origin
        
        if (np.linalg.norm(OC) < self.radius) or (np.dot(OC, dir) < 0):
            return np.inf
        
        l = np.linalg.norm(np.dot(OC, dir))
        
        m_square = np.linalg.norm(OC) * np.linalg.norm(OC) - l * l
        
        q_square = self.radius*self.radius - m_square
        
        return (l - np.sqrt(q_square)) if q_square >= 0 else np.inf

    def get_normal(self, point):
        return normalize(point - self.position)

class Plane(Object):
    
    def __init__(self, position, normal, color=np.array([1.,1.,1.]), reflection=0.15, diffuse=.75, specular_c=.3, specular_k=50):
        super().__init__(position, color, reflection, diffuse, specular_c, specular_k)
        self.normal = np.array(normal)

    def intersect(self, origin, dir):
        dn = np.dot(dir, self.normal)
        if np.abs(dn) < 1e-6:
            return np.inf
        d = np.dot(self.position - origin, self.normal) / dn
        return d if d > 0 else np.inf

    def get_normal(self, point):
        return self.normal

def intersect_color(origin, dir, intensity, scene, light_point, light_color, ambient):

    min_distance = np.inf

    for i, obj in enumerate(scene):
        current_distance = obj.intersect(origin, dir)
        if current_distance < min_distance:
            min_distance, obj_index = current_distance, i

    if min_distance == np.inf or intensity < 0.01:
        return np.array([0., 0., 0.])

    obj = scene[obj_index]
    P = origin + dir * min_distance
    color = obj.get_color()
    N = obj.get_normal(P)
    PL = normalize(light_point - P)
    PO = normalize(origin - P)

    c = ambient * color
    l = [scene[i].intersect(P + N * .0001, PL) for i in range(len(scene)) if i != obj_index]
    if not (l and min(l) < np.linalg.norm(light_point - P)):
        c += obj.diffuse * max(np.dot(N, PL), 0) * color * light_color
        c += obj.specular_c * max(np.dot(N, normalize(PL + PO)), 0) ** obj.specular_k * light_color

    reflect_ray = dir - 2 * np.dot(dir, N) * N
    c += obj.reflection * intersect_color(P + N * .0001, reflect_ray, obj.reflection * intensity, scene, light_point, light_color, ambient)

    return np.clip(c, 0, 1)

def rendering(w, h, scene, O, light_point, light_color, ambient, filename='test.png'):
    img = np.zeros((h, w, 3))
    r = float(w) / h
    S = (-1., -1. / r + .25, 1., 1. / r + .25)
    Q = np.array([0., 0., 0.])
    for i, x in enumerate(np.linspace(S[0], S[2], w)):
        print("%.2f" % (i / float(w) * 100), "%")
        for j, y in enumerate(np.linspace(S[1], S[3], h)):
            Q[:2] = (x, y)
            img[h - j - 1, i, :] = intersect_color(O, normalize(Q - O), 1, scene, light_point, light_color, ambient)
    plt.imsave(filename, img)


def main():
    scene = [
        Sphere([.75, .1, 1.], .6, [.8, .3, 0.]), # position, radius, color
        Sphere([-.3, .01, .2], .3, [.0, .0, .9]), # position, radius, color
        Sphere([-2.75, .1, 3.5], .6, [.1, .572, .184]), # position, radius, color
        Plane([0., -.5, 0.], [0., 1., 0.]) # position, normal
    ]

    light_point = np.array([5., 5., -10.])
    light_color = np.array([1., 1., 1.])
    ambient = 0.05

    w, h = 200, 200
    O = np.array([0., 0.35, -1.])

    # 調用函式
    rendering(w, h, scene, O, light_point, light_color, ambient, 'result_by_py.png')

if __name__ == "__main__":
    main()