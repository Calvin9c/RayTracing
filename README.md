# RayTracing

## Introduction
Ray tracing is a widely-used rendering technique in computer graphics that simulates the behavior of light as it traverses through space by tracking the interaction of light rays with objects in a scene. This GitHub repository implements a straightforward ray tracing renderer. In addition to the serial version of the rendering process, this project also utilizes parallel development libraries such as Pthread, OpenMP, and CUDA for rendering acceleration.

## Quick Start Guide

To get started, follow these simple steps:

1. Clone the Repository:
`git clone https://github.com/Calvin9c/RayTracing.git && cd RayTracing`

2. Install Required Libraries:
Our implementation uses libraries like glm::vec and cv::Mat to enhance the rendering process. Install these essential libraries by running:
`chmod +x install.sh && ./install.sh`

3. Running the Renderer:
Try out different versions of our renderer by following these instructions:
   * Sequential Version:
      * Navigate to the Sequential version directory:
         `cd ./Implement_By_C++/img/Sequential_Version`
      * Compile and run the application:
         `make && ./raytracing`

   * Pthread Version:
      * Navigate to the Pthread version directory:
         `cd ./Implement_By_C++/img/Pthread_Version/Load_Balance`
      * Compile and run the application:
         `make && ./raytracing`

   * OpenMP version:
      * Navigate to the OpenMP version directory:
         `cd ./Implement_By_C++/img/OpenMP_Version`
      * Compile and run the application:
         `make && ./raytracing`

   * CUDA version:
      * Navigate to the CUDA version directory:
         `cd ./Implement_By_C++/img/CUDA_Version`
      * Compile and run the application:
         `make && ./raytracing`

Once the execution is complete, the rendered image will be saved as 'result.png' in the respective directory.

## References
1. [Simple Ray Tracing Renderer](https://ppt.cc/fvCYvx)