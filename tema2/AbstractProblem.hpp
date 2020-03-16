#pragma once

#include <GL/freeglut.h>
#include <string>
#include <functional>

/// Abstract class for homework problems. A new problem class should derive from this class
class AbstractProblem {
protected:
    static AbstractProblem *instance;
    int windowWidth, windowHeight;
private:
    std::string title;
public:
    AbstractProblem(const std::string &title, int windowWidth, int windowHeight)
            : title(title), windowWidth(windowWidth), windowHeight(windowHeight) {}

    void init(int argc, char *argv[]) {
        glutInit(&argc, argv);
        glutSetOption(GLUT_MULTISAMPLE, 8);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
        glutInitWindowPosition(0, 0);
        glutInitWindowSize(windowWidth, windowHeight);
        glutCreateWindow(title.c_str());

        create();

        glutMouseFunc(mouseWrapper);
        glutKeyboardFunc(keyboardWrapper);
        glutKeyboardUpFunc(keyboardUpWrapper);
        glutSpecialFunc(specialKeyboardWrapper);
        glutSpecialUpFunc(specialKeyboardUpWrapper);
        glutDisplayFunc(displayWrapper);
        glutReshapeFunc(reshapeWrapper);
        glutMainLoop();
    }

    virtual ~AbstractProblem() {
        delete AbstractProblem::instance;
    }

    /// Static functions because we cant pass methods to GLUT, it's sucky old C
    static void setInstance(AbstractProblem *abstractProblem) {
        instance = abstractProblem;
    }

    static void displayWrapper() {
        instance->displayFunc();
    }

    static void reshapeWrapper(int width, int height) {
        instance->reshapeFunc(width, height);
    }

    static void mouseWrapper(int button, int state, int x, int y) {
        instance->mouseFunc(button, state, x, y);
    }

    static void keyboardWrapper(unsigned char key, int x, int y) {
        instance->keyboardFunc(key, x, y);
    }

    static void keyboardUpWrapper(unsigned char key, int x, int y) {
        instance->keyboardUpFunc(key, x, y);
    }

    static void specialKeyboardWrapper(int key, int x, int y) {
        instance->specialKeyboardFunc(key, x, y);
    }

    static void specialKeyboardUpWrapper(int key, int x, int y) {
        instance->specialKeyboardUpFunc(key, x, y);
    }

    /// By default this is implemented, though it can be overridden
    virtual void reshapeFunc(int width, int height) {
        windowWidth = width;
        windowHeight = height;
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, 0, height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glutPostRedisplay();
    }

    /// Derived class must override these...
    virtual void create() = 0;

    virtual void displayFunc() = 0;

    virtual void mouseFunc(int button, int state, int x, int y) = 0;

    virtual void keyboardFunc(unsigned char key, int x, int y) = 0;

    virtual void keyboardUpFunc(unsigned char key, int x, int y) = 0;

    virtual void specialKeyboardFunc(unsigned char key, int x, int y) = 0;

    virtual void specialKeyboardUpFunc(unsigned char key, int x, int y) = 0;
};

AbstractProblem *AbstractProblem::instance = nullptr;

