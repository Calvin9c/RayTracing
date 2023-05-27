# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -fPIC -shared -std=c++14

# Python configuration
PYTHON_CONFIG := python3-config
PYTHON_INCLUDE := $(shell $(PYTHON_CONFIG) --includes)

# pybind11 include
PYBIND11_INCLUDE := $(shell python -c "import pybind11; print(pybind11.get_include())")

# Include path for GLM and stb
INCLUDE_PATH = -I/usr/include/glm -I $(CONDA_PREFIX)/include/opencv4 -I $(PYTHON_INCLUDE) -I $(PYBIND11_INCLUDE)

# Libraries
LIBS = -L$(CONDA_PREFIX)/lib -lGL -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

# Source file
SRC = main.cpp

# Output binary
BIN = raytracing.so

all:
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) $(SRC) $(LIBS) -o $(BIN)

clean:
	rm $(BIN)