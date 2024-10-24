# RayTracing

![result](example/example.png)

## Introduction
This directory refactors the previous implementations of C++, Python, and Pybind in this project. We have encapsulated the ray tracing renderer implementation into a package and placed it under the `RayTracing` directory. This refactored version provides an easier way for developers to use the renderer with support for either C++ or Python implementations.

## Installation
Before installation, please ensure that you have Conda installed. Use the following command to install the necessary dependencies, replacing `<conda_env_name>` with the name of the Conda environment you wish to create:
```bash
git clone https://github.com/Calvin9c/RayTracing.git && cd RayTracing/refactor_version
chmod +x install.sh && source install.sh <conda_env_name>
```
This script will install the required dependencies and set up the environment.

## Quick Start Guide
Users can choose to run either `main.cpp` or `main.py` to perform rendering. Below is the guide for usage:

### Running `main.cpp`
Compile and render using the following commands:
```bash
make
./raytracing --img_h 512 --img_w 512 --result_save_path result.png
# Or using the short option names
./raytracing -h 512 -w 512 -r result.png
```

### Running `main.py`
Perform rendering using Python:
```bash
python main.py --img_h 800 --img_w 800 --result_save_path output.png --impl c
```
* `img_h`: Specifies the image height (in pixels).
* `img_w`: Specifies the image width (in pixels).
* `result_save_path`: Specifies the path to save the rendered image.
* `impl`: Specifies the renderer implementation to use:
    * If `py` is selected, the Python-based renderer will be used.
    * If `c` is selected, the C++ renderer wrapped via Pybind will be used for rendering.

## Reference
1. [Simple Ray Tracing Renderer](https://zhuanlan.zhihu.com/p/361144314)