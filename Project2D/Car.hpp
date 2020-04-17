#pragma once

#include "utils/math/Vector.hpp"
#include "utils/Color.hpp"
#include "utils/glUtils.hpp"
#include "utils/math/Circle.hpp"


class Car {
    static Vector2 bodyVertices[8];
    static Vector2 windowVertices[4];
    static Circle circle;
    static Color tireColor, wheelPlateColor;
    Color bodyColor;
    Vector2 position, speed;

public:
    Car(const Vector2 &startPosition, const Color &color) : position(startPosition), bodyColor(color) {
        createWindows();
    }

    Car(float x, float y, const Color &color) : position(x, y), bodyColor(color) {
        createWindows();
    }

    void act(float delta) {
        // Update the position each frame, scaled by frametime
        position += speed * delta;
    }

    void draw() {
        glPushMatrix();
        // Translate the car at the current position
        glTranslate(position);

        // Draw body quads
        glBegin(GL_QUADS);
        {
            glColor(bodyColor);
            for (const auto &vert : bodyVertices)
                glVertex(vert);
            glColor(Color::BLACK);
            for (const auto &vert : windowVertices)
                glVertex(vert);
        }
        glEnd();

        glPushMatrix();
        glTranslatef(windowVertices[2].x + 15 / 2.f, windowVertices[2].y, 0);
        glRotatef(180, 0, 1, 0);
        glTranslatef(-windowVertices[2].x - 15 / 2.f, -windowVertices[2].y, 0);
        glBegin(GL_QUADS);
        glColor(Color::BLACK);
        for (const auto &vert : windowVertices)
            glVertex(vert);
        glEnd();
        glPopMatrix();

        // Draw first wheel
        drawWheel();

        // Draw second wheel
        glTranslatef(180, 0, 0);
        drawWheel();

        glPopMatrix();
    }

    const Vector2 &getSpeed() const {
        return speed;
    }

    void setSpeed(const Vector2 &speed) {
        Car::speed = speed;
    }

private:
    void drawWheel() {
        // Draw tire
        glColor(tireColor);
        circle.draw();

        glPushMatrix();
        glTranslatef(100, 0, 0);
        glScalef(.6f, .6f, 1);
        glTranslatef(-100, 0, 0);
        // Draw plate
        glColor(wheelPlateColor);
        circle.draw();
        glPopMatrix();
    }

    static void createWindows() {
        float offset = 15;
        windowVertices[0].set(bodyVertices[4]).x += offset;
        windowVertices[1].set(bodyVertices[4]).x += bodyVertices[4].dist(bodyVertices[5]) / 2 - offset;
        windowVertices[2].set(windowVertices[1]).y += (bodyVertices[7].y - bodyVertices[4].y) - offset / 2;
        windowVertices[3].set(bodyVertices[7]).y -= offset / 2;
    }
};

Vector2 Car::bodyVertices[8]{
        // Bottom quad
        {0,   0},   //0
        {350, 0},   //1
        {350, 60},  //2
        {0,   60},  //3
        // Top quad
        {50,  60},  //4
        {350, 60},  //5
        {300, 100}, //6
        {100, 100}};//7
Vector2 Car::windowVertices[4]{};
Circle Car::circle{{100, 0}, 30};
Color Car::tireColor{(uint8_t) 40, 40, 40};
Color Car::wheelPlateColor{0.7f, 0.7f, 0.7f};