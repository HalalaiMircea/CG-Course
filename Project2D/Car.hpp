#pragma once

#include "utils/math/Vector.hpp"
#include "utils/Color.hpp"
#include "utils/glUtils.hpp"
#include "utils/math/Circle.hpp"


class Car {
public:
    static constexpr float wheelRadius = 30.0f;

private:
    static Vector2 bodyVertices[8];
    static Vector2 windowVertices[4];
    static Circle circle;
    static Color tireColor, wheelPlateColor;
    Color bodyColor;
    Vector2 position, speed;

public:
    Car() = default;

    Car(const Vector2 &startPosition, const Color &color) : position(startPosition), bodyColor(color) {}

    Car(float x, float y, const Color &color) : position(x, y), bodyColor(color) {}

    void act(float delta) {
        // Update the position each frame, scaled by frametime
        position += speed * delta;
    }

    void draw() {
        glPushMatrix();
        glTranslate(position);        // Translate the car at the current position

        glBegin(GL_QUADS);
        {
            // Draw body quads
            glColor(bodyColor);
            for (const auto &vert : bodyVertices) glVertex(vert);
            // Draw first window quad
            glColor(Color::BLACK);
            for (const auto &vert : windowVertices) glVertex(vert);
        }
        glEnd();
        // Draw second window quad rotated
        glPushMatrix();
        glTranslatef(windowVertices[2].x + 15 / 2.f, windowVertices[2].y, 0);
        glRotatef(180, 0, 1, 0);
        glTranslatef(-windowVertices[2].x - 15 / 2.f, -windowVertices[2].y, 0);
        glBegin(GL_QUADS);
        {
            glColor(Color::BLACK);
            for (const auto &vert : windowVertices) glVertex(vert);
        }
        glEnd();
        glPopMatrix();

        drawWheelTranslated(0, 0);      // Draw first wheel
        drawWheelTranslated(180, 0);    // Draw second wheel

        glPopMatrix();
    }

    const Vector2 &getSpeed() const {
        return speed;
    }

    void setSpeed(const Vector2 &speed) {
        Car::speed = speed;
    }

    static void generateResources() {
        // Bottom quad
        bodyVertices[0] = {0, 0};
        bodyVertices[1] = {350, 0};
        bodyVertices[2] = {350, 60};
        bodyVertices[3] = {0, 60};
        // Top quad
        bodyVertices[4] = {50, 60};
        bodyVertices[5] = {350, 60};
        bodyVertices[6] = {300, 100};
        bodyVertices[7] = {100, 100};

        // Window quad
        float offset = 15;
        windowVertices[0].set(bodyVertices[4]).x += offset;
        windowVertices[1].set(bodyVertices[4]).x += bodyVertices[4].dist(bodyVertices[5]) / 2 - offset;
        windowVertices[2].set(windowVertices[1]).y += (bodyVertices[7].y - bodyVertices[4].y) - offset / 2;
        windowVertices[3].set(bodyVertices[7]).y -= offset / 2;

        circle = {{100, 0}, wheelRadius};
        tireColor = {(uint8_t) 40, 40, 40};
        wheelPlateColor = {0.7f, 0.7f, 0.7f};
    }

    static float getModelWidth() {
        return bodyVertices[0].dist(bodyVertices[1]);
    }

    static float getModelHeight() {
        return bodyVertices[7].y - bodyVertices[0].y + wheelRadius;
    }

    static Vector2 getModelSize() {
        return Vector2(getModelWidth(), getModelHeight());
    }

private:
    static void drawWheelTranslated(float amountX, float amountY) {
        glPushMatrix();
        // Draw tire
        glTranslatef(amountX, amountY, 0);
        glColor(tireColor);
        circle.draw();

        // Draw plate
        glPushMatrix();
        {
            glTranslatef(100, 0, 0);
            glScalef(.6f, .6f, 1);
            glTranslatef(-100, 0, 0);
            glColor(wheelPlateColor);
            circle.draw();
        }
        glPopMatrix();
        glPopMatrix();
    }
};

Vector2 Car::bodyVertices[8]{};
Vector2 Car::windowVertices[4]{};
Circle Car::circle{};
Color Car::tireColor{};
Color Car::wheelPlateColor{};