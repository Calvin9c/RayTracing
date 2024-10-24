#!/bin/bash

if ! command -v conda &> /dev/null
then
    echo "Error: Conda is not installed. Please install Conda and try again."
    exit 1
fi

DEFAULT_ENV_NAME="rt"

if [ $# -eq 0 ]; then
    ENV_NAME=$DEFAULT_ENV_NAME
else
    ENV_NAME=$1
fi

echo "Using conda environment: $ENV_NAME"

echo "Creating conda environment: $ENV_NAME..."
conda create -n $ENV_NAME python=3.10 -y

echo "Activating conda environment: $ENV_NAME..."
source $(conda info --base)/etc/profile.d/conda.sh
conda activate $ENV_NAME

if [ -f "requirements.txt" ]; then
    echo "Installing Python dependencies from requirements.txt..."
    pip install -r requirements.txt
else
    echo "requirements.txt not found! Skipping Python dependencies installation."
fi

echo "Installing libstdcxx-ng..."
conda install -c conda-forge libstdcxx-ng -y

echo "Updating package list..."
sudo apt update

echo "Installing build-essential (gcc, g++)..."
sudo apt install -y build-essential

echo "Installing OpenMP..."
sudo apt install -y libomp-dev

echo "Installing glm..."
sudo apt install -y libglm-dev

echo "Installing OpenCV..."
sudo apt install -y libopencv-dev

echo "Installing pkg-config..."
sudo apt install -y pkg-config

echo "All libraries have been successfully installed and conda environment '$ENV_NAME' is set up!"