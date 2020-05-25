#pragma once

#include "Color.hpp"
#include "math/Vector.hpp"

void glVertex(const Vector2 &v);

void glVertex(const Vector2 &v, const Color &c);

void glVertex(const Vector3 &v);

void glVertex(const Vector3 &v, const Color &c);

void glColor(const Color &color);

void glTranslate(const Vector2 &amount);

void glTranslate(const Vector3 &amount);

void glRect(const Vector2 &leftBottom, const Vector2 &rightTop);

void glRasterPos(const Vector2 &pos);

void glNormal(const Vector3 &norm);

void glTexCoord(const Vector2 &texCoord);