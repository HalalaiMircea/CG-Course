#pragma once

#include <GL/gl.h>

const float PI = 3.14159265358979323846;

class Color {
public:
    float r, g, b;

    Color() : r(0), g(0), b(0) {}

    Color(float r, float g, float b) : r(r), g(g), b(b) {}

    /*float *getArray() const {
        float v[3];
        v[0] = r;
        v[1] = g;
        v[2] = b;
        return v;
    }*/
};

class Vertex {
public:
    float pos[2]{};

    Vertex(float x, float y) {
        pos[0] = x;
        pos[1] = y;
    }

    void draw() {
        glVertex2fv(pos);
    }

    void draw(float red, float green, float blue) {
        glColor3f(red, green, blue);
        glVertex2fv(pos);
    }

    void draw(const Color &new_color) {
        glColor3f(new_color.r, new_color.g, new_color.b);
        glVertex2fv(pos);
    }
};
