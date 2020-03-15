cmake_minimum_required(VERSION 3.15)
project(tema2)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} main.cpp)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} -lfreeglut -lopenGL32 -lGLU32)
else ()
    target_link_libraries(${PROJECT_NAME} -lglut -lGL -lGLU)
endif ()