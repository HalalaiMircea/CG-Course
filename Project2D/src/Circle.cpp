#include "Circle.hpp"
#include "utils/glUtils.hpp"


Circle::Circle(const Vector2 &position, float radius)
        : position(position), radius(radius) {
    computeVertices();
}

void Circle::draw() const {
    glBegin(GL_POLYGON);
    for (const auto &vert : vertices)
        glVertex(vert);
    glEnd();
}

void Circle::computeVertices() {
    for (int i = 0; i < 360; ++i) {
        float th = (float) i * degToRad;
        vertices[i] = Vector2(position.x + radius * std::cos(th),
                              position.y + radius * std::sin(th));
    }
}