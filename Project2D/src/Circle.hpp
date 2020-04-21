#pragma once

#include "utils/math/Vector.hpp"
#include "utils/glUtils.hpp"

class Circle {
    Vector2 vertices[360];
    Vector2 position;
    float radius{};
public:
    Circle() = default;

    Circle(const Vector2 &position, float radius)
            : position(position), radius(radius) {
        computeVertices();
    }

    void draw() const {
        glBegin(GL_POLYGON);
        for (const auto &vert : vertices)
            glVertex(vert);
        glEnd();
    }

    const Vector2 &getPosition() const {
        return position;
    }

    void setPosition(const Vector2 &position) {
        this->position = position;
        // We recalculate the new vertices
        computeVertices();
    }

    float getRadius() const {
        return radius;
    }

    void setRadius(float radius) {
        this->radius = radius;
        computeVertices();
    }

private:
    void computeVertices() {
        for (int i = 0; i < 360; ++i) {
            float th = (float) i * degToRad;
            vertices[i] = Vector2(position.x + radius * std::cos(th),
                                  position.y + radius * std::sin(th));
        }
    }
};
