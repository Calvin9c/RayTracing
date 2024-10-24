# 編譯器
CXX = g++

# 編譯選項
CXXFLAGS = -Wall -std=c++17

# 包含的頭文件目錄
INCLUDES = -I./RayTracing/cpp_impl/include -I/usr/include/opencv4 -I/usr/include/glm

# 庫的連結選項 (這裡使用 OpenCV 和 OpenGL)
LIBS = -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lGL -lGLU -lglut

# OpenCV 和 OpenGL 庫路徑 (如果安裝在預設路徑，這部分可以省略)
# 例如，如果你手動安裝了 OpenCV 或 OpenGL：
# LIBPATHS = -L/usr/local/lib

# 目標檔案 (輸出執行檔)
TARGET = raytracing

# 源文件
SRCS = ./main.cpp ./RayTracing/cpp_impl/src/renderer.cpp ./RayTracing/cpp_impl/src/objects.cpp ./RayTracing/cpp_impl/src/base_object.cpp

# 物件文件 (自動生成)
OBJS = $(SRCS:.cpp=.o)

# 編譯和連結
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# 編譯規則
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 清理編譯過程生成的文件
clean:
	rm -f $(OBJS) $(TARGET)

# 運行目標
run: $(TARGET)
	./$(TARGET)
