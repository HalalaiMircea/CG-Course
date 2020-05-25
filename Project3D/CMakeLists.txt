cmake_minimum_required(VERSION 3.16)
project(Project3D)

set(CMAKE_CXX_STANDARD 14)

# Link flags for each compiler (MinGW and Linux GCC)
if (WIN32)
    set(GL_LIBS -lfreeglut -lopenGL32 -lGLU32)
else ()
    set(GL_LIBS -lglut -lGL -lGLU)
endif ()

add_executable(${PROJECT_NAME} src/main.cpp src/utils/Color.cpp src/utils/GlutApp3D.cpp src/utils/glUtils.cpp src/ObjLoader.cpp src/ObjLoader.hpp)
target_link_libraries(${PROJECT_NAME} ${GL_LIBS})