#pragma once

/// Abstract class for homework problems. A derived class has to implement some methods, others are optional
class AppListener {
public:
    virtual ~AppListener() = default;

    /// Required methods that need to be overridden
    virtual void create() = 0;

    virtual void displayFunc() = 0;

    virtual void reshapeFunc(int width, int height) = 0;

    /// Optional methods
    virtual void mouseFunc(int button, int state, int x, int y) {}

    virtual void keyboardFunc(unsigned char key, int x, int y) {}

    virtual void keyboardUpFunc(unsigned char key, int x, int y) {}

    virtual void specialKeyboardFunc(int key, int x, int y) {}

    virtual void specialKeyboardUpFunc(int key, int x, int y) {}
};