#pragma once

#include "utils/math/Vector.hpp"
#include "utils/Color.hpp"
#include "Circle.hpp"


class Car {
public:
    static constexpr float WHEEL_RADIUS = 30.0f;
private:
    static const Circle circle;
    static const Color tireColor, wheelPlateColor;
    static Vector2 bodyVertices[8];
    static Vector2 windowVertices[4];
    Color bodyColor;
    Vector2 position, speed;

public:
    Car() = default;

    Car(float x, float y, const Color &color);

    void act(float delta);

    void draw();

    void setSpeed(const Vector2 &speed);

    void setPosition(const Vector2 &position);

    const Vector2 &getPosition() const;

    Vector2 getRightPos() const;

    static void generateResources();

    static float getModelWidth();

    static float getModelHeight();

private:
    static void drawWheelTranslated(float amountX, float amountY);
};