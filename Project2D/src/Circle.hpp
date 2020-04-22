#pragma once

#include "utils/math/Vector.hpp"

class Circle {
    Vector2 vertices[360];
    Vector2 position;
    float radius{};
public:
    Circle() = default;

    Circle(const Vector2 &position, float radius);

    void draw() const;

private:
    void computeVertices();
};