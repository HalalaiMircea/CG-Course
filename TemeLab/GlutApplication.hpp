#pragma once

#include <string>
#include <GL/freeglut.h>
#include "IAppListener.hpp"

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

/** Singleton GlutApplication class that gets a specific application's behaviour through listener.
 * First set the AppListener object with setListener static method.
 * Then set a config directly on the public static attribute config
 * Finally run the app with initialize
**/
class GlutApplication {
    static IAppListener *listener;
public:
    static Configuration config;

    GlutApplication(IAppListener *appListener, const Configuration &configuration, int argc, char *argv[]) {
        listener = appListener;
        config = configuration;
        initialize(argc, argv);
    }

    virtual ~GlutApplication() {
    }

    static void exit() {
        glutExit();
    }

private:
    static void initialize(int argc, char **argv) {
        glutInit(&argc, argv);
        glutSetOption(GLUT_MULTISAMPLE, config.samples);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
        if (config.x == -1 || config.y == -1) {
            glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - config.width) / 2,
                                   (glutGet(GLUT_SCREEN_HEIGHT) - config.height) / 2);
        } else glutInitWindowPosition(config.x, config.y);
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

    static void update(int width, int height) {
        config.width = width;
        config.height = height;
    }

    static void displayWrapper() {
        listener->displayFunc();
    }

    static void reshapeWrapper(int width, int height) {
        update(width, height);
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
IAppListener *GlutApplication::listener = nullptr;
