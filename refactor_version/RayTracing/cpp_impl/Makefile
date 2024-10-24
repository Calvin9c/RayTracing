OPENCV_PATH = /usr/include/opencv4
GLM_PATH = /usr/include/glm

# Compiler and flags
CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++17 -fPIC `python3 -m pybind11 --includes` -I$(OPENCV_PATH) -I$(GLM_PATH)
LDFLAGS = -L/usr/local/lib `pkg-config --libs opencv4`
PYTHON_LIB = `python3-config --ldflags`

# Directories
INCLUDE_DIR = include
SRC_DIR = src
PYBIND_DIR = pybind
BUILD_DIR = build

# Source files
RENDERER_SRC = $(PYBIND_DIR)/binding_renderer.cpp $(SRC_DIR)/renderer.cpp $(SRC_DIR)/base_object.cpp
OBJECTS_SRC = $(PYBIND_DIR)/binding_objects.cpp $(SRC_DIR)/objects.cpp $(SRC_DIR)/base_object.cpp
VEC_SRC = $(PYBIND_DIR)/binding_vec.cpp

# Output files
RENDERER_SO = $(BUILD_DIR)/renderer.so
OBJECTS_SO = $(BUILD_DIR)/objects.so
VEC_SO = $(BUILD_DIR)/vec.so

# Targets
all: $(RENDERER_SO) $(OBJECTS_SO) $(VEC_SO)

# Build renderer.so
$(RENDERER_SO): $(RENDERER_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -I$(PYBIND_DIR) -o $@ $^ $(LDFLAGS) $(PYTHON_LIB)

# Build objects.so
$(OBJECTS_SO): $(OBJECTS_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -I$(PYBIND_DIR) -o $@ $^ $(LDFLAGS) $(PYTHON_LIB)

$(VEC_SO): $(VEC_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(VEC_SRC) -o $(VEC_SO) $(PYTHON_LIB)

# Create build directory if not exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
