#pragma once

#include <string>
#include <GL/freeglut.h>
#include "AppListener.hpp"

#undef far
#undef near

struct Configuration {
    /** Title of the application **/
    std::string title;
    /** Number of samples for anti-aliasing **/
    int samples = 0;
    /** Width and height of application window **/
    int width = 640, height = 480;
    /** x & y of application window, -1 for center **/
    int x = -1, y = -1;
    /** Orthogonal boundries of the openGL context. By default they are -1 1 -1 1 -1 1**/
    int orthoLeft = -1, orthoRight = 1, orthoBottom = -1, orthoTop = 1, orthoNear = -1, orthoFar = 1;

    void setOrtho(int left, int right, int botton, int top, int near, int far) {
        orthoLeft = left;
        orthoRight = right;
        orthoBottom = botton;
        orthoTop = top;
        orthoNear = near;
    }
};

/** GlutApplication class that gets a specific application's behaviour through a listener and a configuration object to
 * setup the window accordingly
**/
class GlutApplication {
    static AppListener *listener;
public:
    static Configuration config;

    GlutApplication(AppListener *appListener, const Configuration &configuration, int argc, char *argv[]) {
        listener = appListener;
        config = configuration;
        initialize(argc, argv);
    }

    virtual ~GlutApplication() {
        delete listener;
    }

    static void exit() {
        glutExit();
    }

private:
    static void initialize(int argc, char **argv) {
        glutInit(&argc, argv);
        glutSetOption(GLUT_MULTISAMPLE, config.samples);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
        if (config.x < 0 || config.y < 0) {
            config.x = (glutGet(GLUT_SCREEN_WIDTH) - config.width) / 2;
            config.y = (glutGet(GLUT_SCREEN_HEIGHT) - config.height) / 2;
        }
        glutInitWindowPosition(config.x, config.y);
        glutInitWindowSize(config.width, config.height);
        glutCreateWindow(config.title.c_str());

        listener->reshapeFunc(config.width, config.height);
        listener->create();

        glutMouseFunc(mouseWrapper);
        glutKeyboardFunc(keyboardWrapper);
        glutKeyboardUpFunc(keyboardUpWrapper);
        glutSpecialFunc(specialKeyboardWrapper);
        glutSpecialUpFunc(specialKeyboardUpWrapper);
        glutDisplayFunc(displayWrapper);
        glutReshapeFunc(reshapeWrapper);
        glutMainLoop();
    }

    static void displayWrapper() {
        listener->displayFunc();
        glutSwapBuffers();
        glutPostRedisplay();
    }

    static void reshapeWrapper(int width, int height) {
        config.width = width;
        config.height = height;
        listener->reshapeFunc(width, height);
    }

    static void mouseWrapper(int button, int state, int x, int y) {
        listener->mouseFunc(button, state, x, y);
    }

    static void keyboardWrapper(unsigned char key, int x, int y) {
        listener->keyboardFunc(key, x, y);
    }

    static void keyboardUpWrapper(unsigned char key, int x, int y) {
        listener->keyboardUpFunc(key, x, y);
    }

    static void specialKeyboardWrapper(int key, int x, int y) {
        listener->specialKeyboardFunc(key, x, y);
    }

    static void specialKeyboardUpWrapper(int key, int x, int y) {
        listener->specialKeyboardUpFunc(key, x, y);
    }
};

Configuration GlutApplication::config = {};
AppListener *GlutApplication::listener = nullptr;
