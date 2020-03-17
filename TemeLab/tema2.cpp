#include <iostream>
#include "GlutApplication.hpp"

class Hw2Problem1 : public AppListener {
    float X[100]{}, Y[100]{};
    int vert_count = 0;
public:

    void create() override {
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
    }

    void reshapeFunc(int width, int height) override {
        glViewport(0, 0, GlutApplication::config.width, GlutApplication::config.height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, GlutApplication::config.width, 0, GlutApplication::config.height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void mouseFunc(int button, int state, int x, int y) override {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && vert_count <= 3) {
            X[vert_count] = (float) x;
            Y[vert_count] = (float) (GlutApplication::config.height - y);
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
                    } else std::cout << "Lines don't intersect\n";
                }
            }
            vert_count++;
        }
    }

    void keyboardFunc(unsigned char key, int x, int y) override {
        if (key == 27) //ESC
            GlutApplication::exit();
    }
};

class Hw2Problem2 : public AppListener {
    float X[100]{}, Y[100]{};
    int k = 0;
    bool intersectedLines[100] = {};
public:
    ~Hw2Problem2() override {
        std::cout << "Hw2Problem2 Destroyed\n";
    }

    void create() override {
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
    }

    void reshapeFunc(int width, int height) override {
        glViewport(0, 0, GlutApplication::config.width, GlutApplication::config.height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, GlutApplication::config.width, 0, GlutApplication::config.height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void mouseFunc(int button, int state, int x, int y) override {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            X[k] = (float) x;
            Y[k] = (float) (GlutApplication::config.height - y);
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
            GlutApplication::exit();
    }
};

int main(int argc, char **argv) {
    Configuration config;
    config.width = 800;
    config.height = 600;
    config.title = "Tema 2: Intersectii segmente";
    config.samples = 8;
    config.setOrtho(0, config.width, 0, config.height, -1, 1);

    new GlutApplication(new Hw2Problem2(), config, argc, argv);
    return 0;
}