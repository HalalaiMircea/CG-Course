cmake_minimum_required(VERSION 3.15)
project(TemeLab)

set(CMAKE_CXX_STANDARD 14)

# Link flags for each platform (Windows and Unix)
if (WIN32)
    set(GL_LIBS -lfreeglut -lopenGL32 -lGLU32)
else ()
    set(GL_LIBS -lglut -lGL -lGLU)
endif ()

add_executable(tema1 tema1.cpp)
add_executable(tema2 tema2.cpp IAppListener.hpp GlutApplication.hpp)

target_link_libraries(tema1 ${GL_LIBS})
target_link_libraries(tema2 ${GL_LIBS})