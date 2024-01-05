# RayMarching

## Introduction
Ray tracing is a widely-used rendering technique in computer graphics that simulates the behavior of light as it traverses through space by tracking the interaction of light rays with objects in a scene. This GitHub repository implements a straightforward ray tracing renderer. In addition to the serial version of the rendering process, this project also utilizes parallel development libraries such as Pthread, OpenMP, and CUDA for rendering acceleration.

## Quick Start
`git clone https://github.com/Calvin9c/RayTracing.git && cd RayTracing`

In our implementation, libraries such as glm::vec and cv::Mat are utilized to facilitate the rendering process. Therefore, it is necessary to install these libraries by executing:
`chmod +x install.sh && ./install.sh`

To try our CUDA version of the renderer, please execute the following instructions:
1. Navigate to the CUDA version directory:
   `cd ./Implement_By_C++/img/CUDA_Version`
2. Compile and run the application:
   `make && ./raytracing`

To try our Sequential version of the renderer, please execute the following instructions:
1. Navigate to the Sequential version directory:
   `cd ./Implement_By_C++/img/Sequential_Version`
2. Compile and run the application:
   `make && ./raytracing`

After the execution is complete, the rendered image will be saved as 'result.png'.

## References
1. [Simple Ray Tracing Renderer](https://ppt.cc/fvCYvx)