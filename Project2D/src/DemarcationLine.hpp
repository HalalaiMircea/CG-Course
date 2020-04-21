#pragma once

#include <GL/gl.h>
#include <queue>
#include "utils/math/Vector.hpp"
#include "utils/glUtils.hpp"
#include "utils/GlutApp.hpp"

class DemarcationLine {
public:
    static const Vector2 SPEED, RECT_SIZE;
    static constexpr int RECTANGLES_NUM = 12;
    static constexpr float DIST_IN_BETWEEN = 40;
private:
    Vector2 position;
    std::deque<Vector2> rectanglesPos;

public:
    DemarcationLine() = default;

    explicit DemarcationLine(float yPos) : position(0, yPos) {
        initialize();
    }

    void act(float delta) {
        // We constantly update every rectangle's position
        for (auto &rectPos : rectanglesPos)
            rectPos += SPEED * delta;

        if (rectanglesPos.back().x >= (float) GlutApp::config.orthoRight) {
            rectanglesPos.pop_back();
            rectanglesPos.push_front(position - Vector2(RECT_SIZE.x, 0));
        }

    }

    void draw() {
        glColor(Color::WHITE);
        for (const auto &rectPos : rectanglesPos) {
            glPushMatrix();
            glTranslate(rectPos);  // We translate each rectangle by its position
            glRectf(0, 0, RECT_SIZE.x, RECT_SIZE.y);
            glPopMatrix();
        }
    }

    void setY(float yPos) {
        DemarcationLine::position.y = yPos;
        initialize();
    }

private:
    void initialize() {
        rectanglesPos.clear();
        rectanglesPos.push_back(position - Vector2(RECT_SIZE.x, 0));
        for (int i = 1; i < RECTANGLES_NUM; ++i) {
            rectanglesPos.push_back(rectanglesPos.at(i - 1) + Vector2(RECT_SIZE.x + DIST_IN_BETWEEN, 0));
        }
    }
};

const Vector2 DemarcationLine::SPEED{400, 0};
const Vector2 DemarcationLine::RECT_SIZE{90, 25};
