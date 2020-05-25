#pragma once

#include <string>
#include <chrono>
#include <GL/freeglut.h>
#include <iostream>
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
    /** Field-of-View of the camera **/
    float fov = 75.0f;
};

/** GlutApp3D class that gets a specific application's behaviour through a listener and a configuration object to
 * setup the window accordingly
 */
class GlutApp3D {
public:
    static Configuration config;
private:
    static AppListener *listener;
    static SystemClock::time_point tp1;
    static SystemClock::time_point tp2;
    static float deltaTime;
public:

    GlutApp3D(AppListener *appListener, const Configuration &configuration, int argc, char *argv[]) {
        listener = appListener;
        config = configuration;
        initialize(argc, argv);
    }

    virtual ~GlutApp3D() {
        delete listener;
    }

    static void exit() {
        glutExit();
    }

    static float getDeltaTime();

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
        glutIgnoreKeyRepeat(true);

        listener->create();

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
        tp2 = SystemClock::now();
        std::chrono::duration<float> deltaDuration = tp2 - tp1;
        deltaTime = deltaDuration.count();
        if (deltaTime > 1.0f / 20.0f)
            deltaTime = 1.0f / 20.0f;
        tp1 = tp2;

        listener->render(deltaTime);

        glFlush();
        glutSwapBuffers();
    }

    static void fakeDisplayFunc() {
        // Do nothing so we only use idle func for rendering
    }

    static void reshapeWrapper(int width, int height) {
        config.width = width;
        config.height = height;

        float ratio = (float) width / (float) height;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, width, height);
        gluPerspective(config.fov, ratio, 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);

        listener->resize(width, height);
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