CXX = g++

CXXFLAGS = -Wall -std=c++17

INCLUDES = -I./RayTracing/cpp_impl/include -I/usr/include/opencv4 -I/usr/include/glm

LIBS = -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lGL -lGLU -lglut

TARGET = raytracing

SRCS = ./main.cpp ./RayTracing/cpp_impl/src/renderer.cpp ./RayTracing/cpp_impl/src/objects.cpp ./RayTracing/cpp_impl/src/base_object.cpp

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
