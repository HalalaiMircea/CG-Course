#pragma once

#include <queue>
#include "utils/math/Vector.hpp"

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

    explicit DemarcationLine(float yPos);

    void act(float delta);

    void draw();

    void setY(float yPos);

private:
    void initialize();
};