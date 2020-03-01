cmake_minimum_required(VERSION 3.15)
project(tema1)

set(CMAKE_CXX_STANDARD 14)

add_executable(tema1 main.cpp)
if (WIN32)
    target_link_libraries(tema1 -lfreeGLUT -lopenGL32 -lGLU32)
else ()
    target_link_libraries(tema1 -lglut -lGL -lGLU)
endif ()