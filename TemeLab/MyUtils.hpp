#pragma once

#include <GL/gl.h>
#include <ostream>
#include <cmath>

const float PI = 3.14159265358979323846;
const float degToRad = PI / 180;
const float radToDeg = 180 / PI;

struct Color {
    float r, g, b;

    Color() : r(0), g(0), b(0) {}

    Color(float r, float g, float b) : r(r), g(g), b(b) {}

    static const Color WHITE, BLACK, RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA;
};

const Color Color::BLACK{0, 0, 0};
const Color Color::RED{1, 0, 0};
const Color Color::YELLOW{1, 1, 0};
const Color Color::GREEN{0, 1, 0};
const Color Color::CYAN{0, 1, 1};
const Color Color::BLUE{0, 0, 1};
const Color Color::MAGENTA{1, 0, 1};
const Color Color::WHITE{1, 1, 1};

template<class T>
class Vector {
public:
    virtual float len() = 0;

    virtual T &set(const T &v) = 0;

    virtual T &operator+=(const T &v) = 0;

    virtual T &operator-=(const T &v) = 0;

    virtual T &operator*=(float scalar) = 0;

    virtual T &operator*=(const T &v) = 0;

    virtual T operator+(const T &v) const = 0;

    virtual T operator-(const T &v) const = 0;

    virtual T operator*(const T &v) const = 0;

    virtual T operator*(float scalar) const = 0;

    virtual float dot(const T &v) const = 0;

    virtual float dist(const T &v) const = 0;
};

class Vector2 : public Vector<Vector2> {
public:
    float x, y;

    Vector2() : x(0), y(0) {}

    Vector2(float x, float y) : x(x), y(y) {}

    Vector2(const Vector2 &v) : x(v.x), y(v.y) {}

    float len() override {
        return std::sqrt(x * x + y * y);
    }

    Vector2 &set(const Vector2 &v) override {
        x = v.x;
        y = v.y;
        return *this;
    }

    Vector2 &set(float x, float y) {
        this->x = x;
        this->y = y;
        return *this;
    }

    friend Vector2 operator-(const Vector2 &v) {
        return Vector2(-v.x, -v.y);
    }

    Vector2 &operator+=(const Vector2 &v) override {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2 &operator-=(const Vector2 &v) override {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2 &operator*=(float scalar) override {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2 &operator*=(const Vector2 &v) override {
        x *= v.x;
        y *= v.y;
        return *this;
    }

    Vector2 operator+(const Vector2 &v) const override {
        return Vector2(x + v.x, y + v.y);
    }

    Vector2 operator-(const Vector2 &v) const override {
        return Vector2(x - v.x, y - v.y);
    }

    Vector2 operator*(const Vector2 &v) const override {
        return Vector2(x * v.x, y * v.y);
    }

    Vector2 operator*(float scalar) const override {
        return Vector2(x * scalar, y * scalar);
    }

    float dot(const Vector2 &v) const override {
        return x * v.x + y * v.y;
    }

    /**
     * A method to calculcate the distance between the instance vector and another vector
     * @param v another vector
     * @return the distance
     */
    float dist(const Vector2 &v) const override {
        float x_d = v.x - x;
        float y_d = v.y - y;
        return std::sqrt(x_d * x_d + y_d * y_d);
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector2 &v) {
        os << "(" << v.x << "," << v.y << ")";
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

    Vector3 &set(const Vector3 &v) override {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    Vector3 &set(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
        return *this;
    }

    friend Vector3 operator-(const Vector3 &v) {
        return Vector3(-v.x, -v.y, -v.z);
    }

    Vector3 &operator+=(const Vector3 &v) override {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3 &operator-=(const Vector3 &v) override {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3 &operator*=(float scalar) override {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3 &operator*=(const Vector3 &v) override {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    Vector3 operator+(const Vector3 &v) const override {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator-(const Vector3 &v) const override {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator*(const Vector3 &v) const override {
        return Vector3(x * v.x, y * v.y, z * v.z);
    }

    Vector3 operator*(float scalar) const override {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    float dot(const Vector3 &v) const override {
        return x * v.x + y * v.y + z * v.z;
    }

    /**
     * A method to calculcate the distance between the instance vector and another vector
     * @param v another vector
     * @return the distance
     */
    float dist(const Vector3 &v) const override {
        float x_d = v.x - x,
                y_d = v.y - y,
                z_d = v.z - z;
        return std::sqrt(x_d * x_d + y_d * y_d + z_d * z_d);
    }

    /**
     * Modifies the vector on which you call the method
     * @param vector to do cross product with
     * @return the value of the vector cross is called on
     */
    Vector3 &cross(const Vector3 &vector) {
        set(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
        return *this;
    }

    Vector3 &cross(float x, float y, float z) {
        set(this->y * z - this->z * y, this->z * x - this->x * z, this->x * y - this->y * x);
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector3 &v) {
        os << "(" << v.x << "," << v.y << "," << v.z << ")";
        return os;
    }

    /**
     * Static version of cross method
     * @param v0 - vector 1
     * @param v1 - vector 2
     * @return a new vector resuled from cross product operation
     */
    static Vector3 cross(const Vector3 &v0, const Vector3 &v1) {
        return {v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x};
    }
};

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