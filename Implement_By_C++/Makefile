# Compiler
CC = g++

# Compiler flags
CFLAGS = -std=c++11 -O2 -Wall

# Include path for GLM and stb
INCLUDE_PATH = -I/usr/include/glm -I$(CONDA_PREFIX)/include/opencv4

# Libraries
LIBS = -L$(CONDA_PREFIX)/lib -lGL -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

# Source file
SRC = implement_by.cpp

# Output binary
BIN = raytracer

all: $(SRC)
	export LD_LIBRARY_PATH=$(CONDA_PREFIX)/lib:$$LD_LIBRARY_PATH && \
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $(BIN) $(SRC) $(LIBS)

clean:
	rm $(BIN)