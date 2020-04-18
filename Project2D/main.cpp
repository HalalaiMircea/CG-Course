#include <iostream>
#include <memory>
#include "Car.hpp"
#include "DemarcationLine.hpp"
#include "utils/glUtils.hpp"

using namespace std;

class TrafficScene : public AppListener {
    Vector2 roadLeftBottom, roadRightTop;
    Car staticCar, overcomingCar;
    unique_ptr<DemarcationLine> demarcation;
    GLuint staticScene = 0;
    bool isMessageDisplayed = true;
    string bootUpMessage = "Press SPACEBAR to start demo";

public:
    TrafficScene() = default;

    void create() override {
        roadLeftBottom = {(float) GlutApp::config.orthoLeft, (float) GlutApp::config.orthoTop / 5};
        roadRightTop = {(float) GlutApp::config.orthoRight, (float) GlutApp::config.orthoTop / 2.25f};
        float roadWidth = roadRightTop.x - roadLeftBottom.x;
        float roadHeight = roadRightTop.y - roadLeftBottom.y;
        generateStaticScene();
        demarcation = make_unique<DemarcationLine>(
                roadLeftBottom.y + roadHeight / 2 - DemarcationLine::RECT_SIZE.y / 2);

        Car::generateResources();
        staticCar = Car(roadLeftBottom.x + roadWidth / 2 - Car::getModelWidth() / 2,
                        roadRightTop.y - roadHeight / 2 + Car::getModelHeight() / 2, Color::BLUE);
        overcomingCar = Car(roadWidth + 100, roadLeftBottom.y + roadHeight / 4, Color::RED);
    }

//TODO Draw distant scenery (Trees, mountains, sun...)
    void render(float delta) override {
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update logic
        demarcation->act(delta);
        staticCar.act(delta);
        overcomingCar.act(delta);

        // Draw objects
        glCallList(staticScene);
        demarcation->draw();
        staticCar.draw();
        overcomingCar.draw();
        if (isMessageDisplayed) {
            float textX = (float) GlutApp::config.orthoRight / 2 - (float) (bootUpMessage.length() * 18) / 4;
            float textY = (float) GlutApp::config.orthoTop / 1.25f;
            glutBitmapString(GLUT_BITMAP_HELVETICA_18,
                             reinterpret_cast<const unsigned char *>(bootUpMessage.c_str()));
            glColor(Color::BLACK);
            glRasterPos2f(textX, textY);
        }
    }

    void keyboardDown(unsigned char key, int x, int y) override {
        if (key == ' ' && isMessageDisplayed) {
            cout << "Animation Started!\n";
            overcomingCar.setSpeed({-100, 0});
            isMessageDisplayed = false;
        } else if (key == 27)
            GlutApp::exit();
    }

private:
    void generateStaticScene() {
        staticScene = glGenLists(1);
        glNewList(staticScene, GL_COMPILE);
        {
            glBegin(GL_QUADS);
            // Sky quad
            glColor3ub(200, 240, 255);
            glVertex2f(0, GlutApp::config.orthoTop / 2.25);
            glVertex2f(GlutApp::config.orthoRight, GlutApp::config.orthoTop / 2.25);
            glColor3ub(0, 183, 255);
            glVertex2f(GlutApp::config.orthoRight, GlutApp::config.orthoTop);
            glVertex2f(0, GlutApp::config.orthoTop);
            // Green field quad
            glColor3f(0.604, 0.804, 0.196);
            glVertex2f(0, 0);
            glVertex2f(GlutApp::config.orthoRight, 0);
            glColor3f(0, 0.392, 0);
            glVertex2f(GlutApp::config.orthoRight, GlutApp::config.orthoTop / 1.7);
            glVertex2f(0, GlutApp::config.orthoTop / 1.7);
            glEnd();

            glColor(Color::BLACK);
            glRect(roadLeftBottom, roadRightTop);
        }
        glEndList();
    }
};

int main(int argc, char *argv[]) {
    Configuration config;
    config.width = 1400;
    config.height = 900;
    config.title = "Traffic Overcome";
    config.samples = 8;
    config.setOrtho(0, 1400, 0, 900, -1, 1);

    make_unique<GlutApp>(new TrafficScene(), config, argc, argv);

    return 0;
}
