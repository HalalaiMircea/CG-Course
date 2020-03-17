#pragma once

/// Interface for homework problems. A new problem class should derive from this class
class IAppListener {
public:
    virtual void create() = 0;

    virtual void reshapeFunc(int width, int height) = 0;

    virtual void displayFunc() = 0;

    virtual void mouseFunc(int button, int state, int x, int y) = 0;

    virtual void keyboardFunc(unsigned char key, int x, int y) = 0;

    virtual void keyboardUpFunc(unsigned char key, int x, int y) = 0;

    virtual void specialKeyboardFunc(int key, int x, int y) = 0;

    virtual void specialKeyboardUpFunc(int key, int x, int y) = 0;
};