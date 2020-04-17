cmake_minimum_required(VERSION 3.15)
project(Template)

set(CMAKE_CXX_STANDARD 14)

# Link flags for each platform (Windows and Unix)
if (WIN32)
    set(GL_LIBS -lfreeglut -lopenGL32 -lGLU32)
else ()
    set(GL_LIBS -lglut -lGL -lGLU)
endif ()

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${GL_LIBS})