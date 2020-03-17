#pragma once

#include <GL/gl.h>

const float PI = 3.14159265358979323846;
const float degToRad = PI / 180;
const float radToDeg = 180 / PI;

class Color {
public:
    float r, g, b;

    Color() : r(0), g(0), b(0) {}

    Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

class Vertex {
public:
    float x, y;

    Vertex() {
        x = 0;
        y = 0;
    }

    Vertex(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void draw() const {
        glVertex2f(x, y);
    }

    void draw(float red, float green, float blue) const {
        glColor3f(red, green, blue);
        glVertex2f(x, y);
    }

    void draw(const Color &new_color) const {
        glColor3f(new_color.r, new_color.g, new_color.b);
        glVertex2f(x, y);
    }
};
