#pragma once

#include <GL/gl.h>
#include <ostream>

const float PI = 3.14159265358979323846;
const float degToRad = PI / 180;
const float radToDeg = 180 / PI;

struct Color {
    float r, g, b;

    Color() : r(0), g(0), b(0) {}

    Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

/*struct Vertex {
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

    friend std::ostream &operator<<(std::ostream &os, const Vertex &vertex) {
        os << "x: " << vertex.x << " y: " << vertex.y;
        return os;
    }
};*/


template<class T>
class Vector {
public:
    virtual float len() = 0;

    virtual T *set(const T &v) = 0;

    virtual T *sub(const T &v) = 0;

    virtual T *add(const T &v) = 0;

    virtual float dot(const T &v) const = 0;

    virtual T *scl(float scalar) = 0;

    virtual T *scl(const T &v) = 0;

    virtual float dst(const T &v) const = 0;

};

class Vector2 : public Vector<Vector2> {
public:
    float x, y;

    Vector2() : x(0), y(0) {
    }

    Vector2(float x, float y) : x(x), y(y) {}

    Vector2(const Vector2 &v) : x(v.x), y(v.y) {}

    float len() override {
        return std::sqrt(x * x + y * y);
    }

    Vector2 *set(const Vector2 &v) override {
        x = v.x;
        y = v.y;
        return this;
    }

    Vector2 *set(float x, float y) {
        this->x = x;
        this->y = y;
        return this;
    }

    Vector2 *sub(const Vector2 &v) override {
        x -= v.x;
        y -= v.y;
        return this;
    }

    Vector2 *add(const Vector2 &v) override {
        x += v.x;
        y += v.y;
        return this;
    }

    float dot(const Vector2 &v) const override {
        return x * v.x + y * v.y;
    }

    Vector2 *scl(float scalar) override {
        x *= scalar;
        y *= scalar;
        return this;
    }

    Vector2 *scl(const Vector2 &v) override {
        x *= v.x;
        y *= v.y;
        return this;
    }

    float dst(const Vector2 &v) const override {
        float x_d = v.x - x;
        float y_d = v.y - y;
        return std::sqrt(x_d * x_d + y_d * y_d);
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector2 &v) {
        os << "(" << v.x << " " << v.y << ")";
        return os;
    }
};

class Vector3 : public Vector<Vector3> {
public:
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}

    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3(const Vector3 &v) : x(v.x), y(v.y), z(v.z) {}

    float len() override {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 *set(const Vector3 &v) override {
        x = v.x;
        y = v.y;
        z = v.z;
        return this;
    }

    Vector3 *set(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
        return this;
    }

    Vector3 *sub(const Vector3 &v) override {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return this;
    }

    Vector3 *add(const Vector3 &v) override {
        x += v.x;
        y += v.y;
        z += v.z;
        return this;
    }

    float dot(const Vector3 &v) const override {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3 *scl(float scalar) override {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return this;
    }

    Vector3 *scl(const Vector3 &v) override {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return this;
    }

    float dst(const Vector3 &v) const override {
        float x_d = v.x - x,
                y_d = v.y - y,
                z_d = v.z - z;
        return std::sqrt(x_d * x_d + y_d * y_d + z_d * z_d);
    }

    Vector3 *cross(const Vector3 &vector) {
        return this->set(
                Vector3(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x));
    }

    Vector3 *cross(float x, float y, float z) {
        return this->set(this->y * z - this->z * y, this->z * x - this->x * z, this->x * y - this->y * x);
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector3 &v) {
        os << "(" << v.x << " " << v.y << " " << v.z << ")";
        return os;
    }
};

void glVertex(const Vector2 &v) {
    glVertex2f(v.x, v.y);
}

void glVertex(const Vector3 &v) {
    glVertex3f(v.x, v.y, v.z);
}

void glColor(const Color &color) {
    glColor3f(color.r, color.g, color.b);
}