import numpy as np

def norm(x):
    return np.linalg.norm(x)

def normalize(x):
    return x / norm(x)