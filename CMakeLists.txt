cmake_minimum_required(VERSION 3.17)  # cmake 版本声明
project(FirstEngine)  # 工程名

set(CMAKE_CXX_STANDARD 17)  # C++版本

set(CMAKE_BUILD_TYPE Debug)

add_executable(FirstEngine  # 生成的exe名，以及包含的cpp文件
    src/main.cpp
    src/Renderer.cpp
    src/FrameBuffer.cpp
    src/MathTypes.cpp
    src/Model.cpp
    src/Camera.cpp
)