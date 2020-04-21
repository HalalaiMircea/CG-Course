#pragma once

/// Abstract class for homework problems. A derived class has to implement some methods, others are optional
class AppListener {
public:
    virtual ~AppListener() = default;

    /**
     * Method used by client to instantiate resources. DON'T instantiate at declaration as GlutApp
     * won't be fully initialized at that time.
     */
    virtual void create() = 0;

    /**
     * Render loop where user does logic and drawing
     * @param delta - time in seconds since last frame
     */
    virtual void render(float delta) = 0;

    /// Inheriting class must update ortho values in config, boilerplate is handled in the wrapper function
    virtual void resize(int width, int height) {}

    /**
     * Method which gets called when mouse button clicks are registered
     * @param button - Button identifier, found in glut.h header
     * @param state - Button state (UP or DOWN)
     * @param x - X position of mouse cursor
     * @param y - Y position of mouse cursor
     */
    virtual void mouseFunc(int button, int state, int x, int y) {}

    virtual void keyboardDown(unsigned char key, int x, int y) {}

    virtual void keyboardUp(unsigned char key, int x, int y) {}

    virtual void specialKeyboardDown(int key, int x, int y) {}

    virtual void specialKeyboardUp(int key, int x, int y) {}
};