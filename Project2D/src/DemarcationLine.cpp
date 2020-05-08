#include "DemarcationLine.hpp"
#include "utils/GlutApp.hpp"
#include "utils/glUtils.hpp"

const Vector2 DemarcationLine::SPEED{400, 0};
const Vector2 DemarcationLine::RECT_SIZE{90, 25};

DemarcationLine::DemarcationLine(float yPos) : position(0, yPos) {
    initialize();
}

void DemarcationLine::act(float delta) {
    if (delta > 1.0f / 20.0f) {
        delta = 1.0f / 20.0f;
    }
    // We constantly update every rectangle's position
    for (auto &rectPos : rectanglesPos)
        rectPos += SPEED * delta;

    if (rectanglesPos.back().x >= (float) GlutApp::config.orthoRight) {
        rectanglesPos.pop_back();
        rectanglesPos.push_front(position - Vector2(RECT_SIZE.x, 0));
    }

}

void DemarcationLine::draw() {
    glColor(Color::WHITE);
    for (const auto &rectPos : rectanglesPos) {
        glPushMatrix();
        glTranslate(rectPos);  // We translate each rectangle by its position
        glRectf(0, 0, RECT_SIZE.x, RECT_SIZE.y);
        glPopMatrix();
    }
}

void DemarcationLine::setY(float yPos) {
    DemarcationLine::position.y = yPos;
    initialize();
}

void DemarcationLine::initialize() {
    rectanglesPos.clear();
    rectanglesPos.push_back(position - Vector2(RECT_SIZE.x, 0));
    for (int i = 1; i < RECTANGLES_NUM; ++i) {
        rectanglesPos.push_back(rectanglesPos.at(i - 1) + Vector2(RECT_SIZE.x + DIST_IN_BETWEEN, 0));
    }
}