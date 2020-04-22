#pragma once

#include <string>
#include <chrono>
#include <GL/freeglut.h>
#include "AppListener.hpp"

#undef far
#undef near

typedef std::chrono::system_clock SystemClock;

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

/** GlutApp class that gets a specific application's behaviour through a listener and a configuration object to
 * setup the window accordingly
 */
class GlutApp {
public:
    static Configuration config;
private:
    static AppListener *listener;
    static SystemClock::time_point tp1;
    static SystemClock::time_point tp2;
public:

    GlutApp(AppListener *appListener, const Configuration &configuration, int argc, char *argv[]) {
        listener = appListener;
        config = configuration;
        initialize(argc, argv);
    }

    virtual ~GlutApp() {
        delete listener;
    }

    static void exit() {
        glutExit();
    }

private:
    static void initialize(int argc, char **argv) {
        glutInit(&argc, argv);
        glutSetOption(GLUT_MULTISAMPLE, config.samples);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
        if (config.x < 0 || config.y < 0) {
            config.x = (glutGet(GLUT_SCREEN_WIDTH) - config.width) / 2;
            config.y = (glutGet(GLUT_SCREEN_HEIGHT) - config.height) / 2;
        }
        glutInitWindowPosition(config.x, config.y);
        glutInitWindowSize(config.width, config.height);
        glutCreateWindow(config.title.c_str());

        setOrthoSpace();
        listener->create();
        glutIgnoreKeyRepeat(1);

        glutMouseFunc(mouseWrapper);
        glutKeyboardFunc(keyboardWrapper);
        glutKeyboardUpFunc(keyboardUpWrapper);
        glutSpecialFunc(specialKeyboardWrapper);
        glutSpecialUpFunc(specialKeyboardUpWrapper);
        glutDisplayFunc(fakeDisplayFunc);
        glutIdleFunc(renderWrapper);
        glutReshapeFunc(reshapeWrapper);
        glutMainLoop();
    }

    static void renderWrapper() {
        using namespace std::chrono;
        tp2 = system_clock::now();
        duration<float> deltaTime = tp2 - tp1;
        tp1 = tp2;
        listener->render(deltaTime.count());
        glFlush();
        glutSwapBuffers();
    }

    static void fakeDisplayFunc() {
        // Do nothing so we only use idle func for rendering
    }

    static void reshapeWrapper(int width, int height) {
        config.width = width;
        config.height = height;

        listener->resize(width, height);   // User defined method called using polymorphism

        glViewport(0, 0, config.width, config.height);
        setOrthoSpace();
    }

    static void setOrthoSpace() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(config.orthoLeft, config.orthoRight, config.orthoBottom, config.orthoTop, config.orthoNear,
                config.orthoFar);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    static void mouseWrapper(int button, int state, int x, int y) {
        listener->mouseFunc(button, state, x, y);
    }

    static void keyboardWrapper(unsigned char key, int x, int y) {
        listener->keyboardDown(key, x, y);
    }

    static void keyboardUpWrapper(unsigned char key, int x, int y) {
        listener->keyboardUp(key, x, y);
    }

    static void specialKeyboardWrapper(int key, int x, int y) {
        listener->specialKeyboardDown(key, x, y);
    }

    static void specialKeyboardUpWrapper(int key, int x, int y) {
        listener->specialKeyboardUp(key, x, y);
    }
};