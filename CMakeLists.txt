cmake_minimum_required(VERSION 3.17)  # cmake 版本声明
project(FirstEngine)  # 工程名

set(CMAKE_CXX_STANDARD 17)  # C++版本

add_executable(FirstEngine  # 生成的exe名，以及包含的cpp文件
    src/main.cpp
    src/Renderer.cpp
    src/FrameBuffer.cpp
)