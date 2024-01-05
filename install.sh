#!/bin/bash

echo "Updating system packages..."
sudo apt update
sudo apt upgrade -y

echo "Installing GCC for OpenMP support..."
sudo apt install gcc -y

echo "Installing OpenGL development libraries..."
sudo apt install freeglut3-dev mesa-common-dev -y

echo "Installing OpenCV for C++ development..."
sudo apt install libopencv-dev -y

echo "Installation complete!"