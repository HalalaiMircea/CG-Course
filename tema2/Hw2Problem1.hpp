#pragma once

#include <iostream>
#include "AbstractProblem.hpp"

class Hw2Problem1 : public AbstractProblem {
    float X[100], Y[100];
    int vert_count = 0;
public:
    Hw2Problem1(const std::string &title, int windowWidth = 800, int windowHeight = 600)
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

        glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        for (int i = 0; i < vert_count; i++)
            glVertex2f(X[i], Y[i]);
        glEnd();

        glBegin(GL_POINTS);
        glColor3f(1, 0, 0);
        for (int i = 0; i < vert_count; i++) {
            glVertex2f(X[i], Y[i]);
            if (i == 4 && X[4] != 0 && Y[4] != 0) {
                glColor3f(0, 0, 1);
                glVertex2f(X[4], Y[4]);
            }
        }
        glEnd();

        glutSwapBuffers();
        glutPostRedisplay();
    }

    void mouseFunc(int button, int state, int x, int y) override {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && vert_count <= 3) {
            X[vert_count] = x;
            Y[vert_count] = windowHeight - y;
            vert_count++;
        }
        if (vert_count == 4) {
            float fc = ((X[2] - X[0]) / (X[1] - X[0])) * (Y[1] - Y[0]) - (Y[2] - Y[0]);
            float fd = ((X[3] - X[0]) / (X[1] - X[0])) * (Y[1] - Y[0]) - (Y[3] - Y[0]);
            if (fc * fd < 0) {
                float a1 = Y[1] - Y[0];
                float b1 = X[0] - X[1];
                float c1 = a1 * X[0] + b1 * Y[0];

                float a2 = Y[3] - Y[2];
                float b2 = X[2] - X[3];
                float c2 = a2 * X[2] + b2 * Y[2];

                float denom = a1 * b2 - a2 * b1;

                if (denom != 0) {
                    float xi = (b2 * c1 - b1 * c2) / denom;
                    float yi = (a1 * c2 - a2 * c1) / denom;
                    if ((xi <= std::max(X[0], X[1]) && xi >= std::min(X[0], X[1]) &&
                         yi <= std::max(Y[0], Y[1]) && yi >= std::min(Y[0], Y[1]) &&
                         xi <= std::max(X[2], X[3]) && xi >= std::min(X[2], X[3]) &&
                         yi <= std::max(Y[2], Y[3]) && yi >= std::min(Y[2], Y[3])) == 1) {
                        X[4] = xi;
                        Y[4] = yi;
                        std::cout << "Ends of segments\n";
                        for (int i = 0; i < 4; i++)
                            std::cout << "x=" << X[i] << " y=" << Y[i] << '\n';
                        std::cout << "Lines intersect at x=" << xi << " y=" << yi << std::endl;
                    }
                }
            } else std::cout << "Lines don't intersect\n";
            vert_count++;
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
