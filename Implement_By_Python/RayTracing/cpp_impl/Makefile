# Compiler and flags
CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++17 -fPIC `python3 -m pybind11 --includes` -I/usr/include/opencv4 -I/usr/include/glm
LDFLAGS = -L/usr/local/lib `pkg-config --libs opencv4`
PYTHON_LIB = `python3-config --ldflags`

# Directories
INCLUDE_DIR = include
SRC_DIR = src
PYBIND_DIR = pybind
BUILD_DIR = build

# Source files
RENDERER_SRC = $(PYBIND_DIR)/binding_renderer.cpp $(SRC_DIR)/renderer.cpp
OBJECTS_SRC = $(PYBIND_DIR)/binding_objects.cpp $(SRC_DIR)/objects.cpp

# Output files
RENDERER_SO = $(BUILD_DIR)/renderer.so
OBJECTS_SO = $(BUILD_DIR)/objects.so

# Targets
all: $(RENDERER_SO) $(OBJECTS_SO)

# Build renderer.so
$(RENDERER_SO): $(RENDERER_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -I$(PYBIND_DIR) -o $@ $^ $(LDFLAGS) $(PYTHON_LIB)

# Build objects.so
$(OBJECTS_SO): $(OBJECTS_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -I$(PYBIND_DIR) -o $@ $^ $(LDFLAGS) $(PYTHON_LIB)

# Create build directory if not exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
