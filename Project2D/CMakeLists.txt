cmake_minimum_required(VERSION 3.15)
project(Project2D)

set(CMAKE_CXX_STANDARD 14)

# Link flags for each platform (MinGW and Linux)
if (WIN32)
    set(GL_LIBS -lfreeglut -lopenGL32 -lGLU32)
else ()
    set(GL_LIBS -lglut -lGL -lGLU)
endif ()

add_executable(Project2D src/main.cpp src/utils/AppListener.hpp src/utils/GlutApp.hpp
        src/Car.hpp src/utils/glUtils.hpp src/Circle.hpp src/DemarcationLine.hpp)
target_link_libraries(Project2D ${GL_LIBS})