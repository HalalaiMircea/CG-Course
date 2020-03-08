#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

int WND_WIDTH = 800, WND_HEIGHT = 600;

float X[100], Y[100];
int vert_count = 0;

int k = 0;
bool intersectedLines[100];


void mouseFunction(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && vert_count <= 3) {
        X[vert_count] = x;
        Y[vert_count] = WND_HEIGHT - y;
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
                if ((xi <= max(X[0], X[1]) && xi >= min(X[0], X[1]) &&
                     yi <= max(Y[0], Y[1]) && yi >= min(Y[0], Y[1]) &&
                     xi <= max(X[2], X[3]) && xi >= min(X[2], X[3]) &&
                     yi <= max(Y[2], Y[3]) && yi >= min(Y[2], Y[3])) == 1) {
                    X[4] = xi;
                    Y[4] = yi;
                    cout << "Ends of segments\n";
                    for (int i = 0; i < 4; i++)
                        cout << "x=" << X[i] << " y=" << Y[i] << '\n';
                    cout << "Lines intersect at x=" << xi << " y=" << yi << endl;
                }
            }
        } else cout << "Lines don't intersect\n";
        vert_count++;
    }
}

void render() {
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
}

void mouseFunction2(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        X[k] = x;
        Y[k] = WND_HEIGHT - y;
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
                    if ((xi <= max(X[i], X[i + 1]) && xi >= min(X[i], X[i + 1]) &&
                         yi <= max(Y[i], Y[i + 1]) && yi >= min(Y[i], Y[i + 1]) &&
                         xi <= max(X[k - 1], X[k]) && xi >= min(X[k - 1], X[k]) &&
                         yi <= max(Y[k - 1], Y[k]) && yi >= min(Y[k - 1], Y[k])) == 1) {
                        intersectedLines[i] = true;
                        intersectedLines[k - 1] = true;
                    }
                }
            }
        }
        k++;
    }
}

void render2() {
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
}

void wndReshapeFunc(int width, int height) {
    WND_WIDTH = width;
    WND_HEIGHT = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
}

void enableMultisample(bool msaa_enabled) {
    if (msaa_enabled) {
        glEnable(GL_MULTISAMPLE);
        glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

        // detect current settings
        GLint iMultiSample = 0;
        GLint iNumSamples = 0;
        glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
        glGetIntegerv(GL_SAMPLES, &iNumSamples);
        printf("MSAA on, GL_SAMPLE_BUFFERS = %d, GL_SAMPLES = %d\n", iMultiSample, iNumSamples);
    } else {
        glDisable(GL_MULTISAMPLE);
        printf("MSAA off\n");
    }
}

void init() {
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WND_WIDTH, 0.0, WND_HEIGHT);
    enableMultisample(true);

    glLineWidth(3);
    glPointSize(10);
    glEnable(GL_POINT_SMOOTH);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WND_WIDTH, WND_HEIGHT);
    glutCreateWindow("Tema 2: Intersectii de segmente");

    std::string option;
    cout << "Problem 1 or 2?\n";
    cin >> option;
    while (option != "1" && option != "2") {
        cout << "INVALID OPTION\n";
        cin >> option;
    }

    init();
    if (option == "1") {
        glutMouseFunc(mouseFunction);
        glutDisplayFunc(render);
    } else {
        glutMouseFunc(mouseFunction2);
        glutDisplayFunc(render2);
    }
    glutReshapeFunc(wndReshapeFunc);
    glutMainLoop();
}