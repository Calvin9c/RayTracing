# Compiler settings
CC = g++
NVCC = nvcc

# Include path for GLM and stb
INCLUDE_PATH = -I/usr/include/glm -I/usr/include/opencv4 -I/usr/local/cuda-12/include

# Libraries
LIBS = -lGL -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
CUDA_LIBS = -L/usr/local/cuda-12/lib64 -lcudart

# Compiler flags
CFLAGS = -std=c++11 $(INCLUDE_PATH) -O2 -Wall
NVCCFLAGS = -Xcompiler -fPIC $(INCLUDE_PATH)

# Target, Output binary
BIN = raytracing

# Source and object files
SRC = main.cpp graph.cu
OBJ = main.o graph.o

# Default target
all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LIBS) $(CUDA_LIBS)

# Generic rule for compiling .cpp to .o
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for compiling .cu to .o
%.o: %.cu
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(BIN) $(OBJ)

# Phony targets
.PHONY: all clean