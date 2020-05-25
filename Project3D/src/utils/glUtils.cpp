#include "glUtils.hpp"
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

void glNormal(const Vector3 &norm) {
    glNormal3f(norm.x, norm.y, norm.z);
}

void glTexCoord(const Vector2 &texCoord) {
    glTexCoord2f(texCoord.x, texCoord.y);
}
