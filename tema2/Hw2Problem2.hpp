#pragma once

#include <iostream>
#include "AbstractProblem.hpp"

class Hw2Problem2 : public AbstractProblem {
    float X[100], Y[100];
    int k = 0;
    bool intersectedLines[100] = {};
public:
    Hw2Problem2(const std::string &title, int windowWidth = 800, int windowHeight = 600)
            : AbstractProblem(title, windowWidth, windowHeight) {}

    void create() override {
        glMatrixMode(GL_PROJECTION);
        gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);

        glLineWidth(3);
        glPointSize(10);
        glEnable(GL_POINT_SMOOTH);
    }

    void displayFunc() override {
        glClearColor(0, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_LINE_STIPPLE);

        glBegin(GL_LINES);
        for (int i = 0; i < k - 1; i++) {
            if (!intersectedLines[i]) {
                glColor3f(1.0, 1.0, 0.0);
                glVertex2f(X[i + 1], Y[i + 1]);
                glVertex2f(X[i], Y[i]);
            }
        }
        glEnd();

        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);

        glBegin(GL_LINES);
        for (int i = 0; i < k - 1; i++) {
            if (intersectedLines[i]) {
                glColor3f(0.0, 0.0, 1.0);
                glVertex2f(X[i + 1], Y[i + 1]);
                glVertex2f(X[i], Y[i]);
            }
        }
        glEnd();

        glutSwapBuffers();
        glutPostRedisplay();
    }

    void mouseFunc(int button, int state, int x, int y) override {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            X[k] = x;
            Y[k] = windowHeight - y;
            for (int i = 0; i < k - 2; i++) {
                float fc = ((X[k - 1] - X[i]) / (X[i + 1] - X[i])) * (Y[i + 1] - Y[i]) - (Y[k - 1] - Y[i]);
                float fd = ((X[k] - X[i]) / (X[i + 1] - X[i])) * (Y[i + 1] - Y[i]) - (Y[k] - Y[i]);
                std::cout << fc << ' ' << fd << '\n';
                if (fc * fd < 0) {
                    float a1 = Y[i + 1] - Y[i];
                    float b1 = X[i] - X[i + 1];
                    float c1 = a1 * X[i] + b1 * Y[i];

                    float a2 = Y[k] - Y[k - 1];
                    float b2 = X[k - 1] - X[k];
                    float c2 = a2 * X[k - 1] + b2 * Y[k - 1];

                    float denom = a1 * b2 - a2 * b1;

                    if (denom != 0) {
                        float xi = (b2 * c1 - b1 * c2) / denom;
                        float yi = (a1 * c2 - a2 * c1) / denom;
                        if ((xi <= std::max(X[i], X[i + 1]) && xi >= std::min(X[i], X[i + 1]) &&
                             yi <= std::max(Y[i], Y[i + 1]) && yi >= std::min(Y[i], Y[i + 1]) &&
                             xi <= std::max(X[k - 1], X[k]) && xi >= std::min(X[k - 1], X[k]) &&
                             yi <= std::max(Y[k - 1], Y[k]) && yi >= std::min(Y[k - 1], Y[k])) == 1) {
                            intersectedLines[i] = true;
                            intersectedLines[k - 1] = true;
                        }
                    }
                }
            }
            k++;
        }
    }

    void keyboardFunc(unsigned char key, int x, int y) override {
        if (key == 27) //ESC
            glutExit();
    }

    void keyboardUpFunc(unsigned char key, int x, int y) override {

    }

    void specialKeyboardFunc(unsigned char key, int x, int y) override {

    }

    void specialKeyboardUpFunc(unsigned char key, int x, int y) override {

    }
};

