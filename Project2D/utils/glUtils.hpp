#pragma once

#include "Color.hpp"
#include "math/Vector.hpp"
#include <GL/gl.h>

void glVertex(const Vector2 &v) {
    glVertex2f(v.x, v.y);
}

void glVertex(const Vector2 &v, const Color &c) {
    glColor3f(c.r, c.g, c.b);
    glVertex2f(v.x, v.y);
}

void glVertex(const Vector3 &v) {
    glVertex3f(v.x, v.y, v.z);
}

void glVertex(const Vector3 &v, const Color &c) {
    glColor3f(c.r, c.g, c.b);
    glVertex3f(v.x, v.y, v.z);
}

void glColor(const Color &color) {
    glColor3f(color.r, color.g, color.b);
}

void glTranslate(const Vector2 &amount) {
    glTranslatef(amount.x, amount.y, 0);
}

void glTranslate(const Vector3 &amount) {
    glTranslatef(amount.x, amount.y, amount.z);
}

void glRect(const Vector2 &leftBottom, const Vector2 &rightTop) {
    glRectf(leftBottom.x, leftBottom.y, rightTop.x, rightTop.y);
}

void glRasterPos(const Vector2 &pos) {
    glRasterPos2f(pos.x, pos.y);
}