#include <iostream>
#include <memory>
#include "Car.hpp"
#include "DemarcationLine.hpp"

using namespace std;

class TrafficScene : public AppListener {
public:
    TrafficScene() = default;

    void create() override {
        initializeTextsPositions();
        initializeDemarcationLine();
        createStaticSceneDisplayLists();

        Car::generateResources();
        staticCar = Car(roadSize.x * 3 / 4 - Car::getModelWidth() / 2,
                        roadRightTop.y - (roadSize.y - Car::getModelHeight()) / 2, Color::BLUE);
        overcomingCar = Car(roadSize.x + 100, roadLeftBottom.y + roadSize.y / 4, Color::RED);
    }

    void render(float delta) override {
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update logic
        demarcation.act(delta);
        overcomingCar.act(delta);
        if (animState == BOTTOM_WEST && overcomingCar.getRightPos().x <= staticCar.getPosition().x) {
            overcomingCar.setSpeed({-100, 50});
            animState = NORTH_WEST;
        } else if (animState == NORTH_WEST && overcomingCar.getPosition().y >= staticCar.getPosition().y) {
            overcomingCar.setSpeed({-100, 0});
            animState = TOP_WEST;
        } else if (animState == TOP_WEST && overcomingCar.getRightPos().x <= roadLeftBottom.x) {
            overcomingCar.setSpeed({0, 0});
            overcomingCar.setPosition({roadSize.x + 100, roadLeftBottom.y + roadSize.y / 4});
            animState = START;
        }

        // Draw objects
        glCallList(staticScene);
        demarcation.draw();
        staticCar.draw();
        overcomingCar.draw();
        if (animState == START)
            glCallList(staticScene + 1);
    }

    void keyboardDown(unsigned char key, int x, int y) override {
        if (animState == START) {
            overcomingCar.setSpeed({-100, 0});
            animState = BOTTOM_WEST;
        }
        if (key == 27)
            GlutApp::exit();
    }

private:
    void initializeTextsPositions() {
        for (int i = 0; i < 2; ++i) {
            float textX = (float) GlutApp::config.orthoRight / 2 -
                          (float) glutBitmapLength(font, (const unsigned char *) texts[i].c_str()) / 2;
            float textY = (float) GlutApp::config.orthoTop / 1.25f - (float) (glutBitmapHeight(font) * i * 5);
            textsPosition[i] = Vector2(textX, textY);
        }
    }

    void initializeDemarcationLine() {
        roadLeftBottom = Vector2((float) GlutApp::config.orthoLeft, (float) GlutApp::config.orthoTop / 5);
        roadRightTop = Vector2((float) GlutApp::config.orthoRight, (float) GlutApp::config.orthoTop / 2.25f);
        roadSize = roadRightTop - roadLeftBottom;

        demarcation.setY(roadLeftBottom.y + (roadSize.y - DemarcationLine::RECT_SIZE.y) / 2);
    }

    void createStaticSceneDisplayLists() {
        staticScene = glGenLists(2);
        glNewList(staticScene, GL_COMPILE);
        {
            glBegin(GL_QUADS);
            // Sky quad
            glColor3ub(200, 240, 255);
            glVertex2f(0, GlutApp::config.orthoTop / 1.7);
            glVertex2f(GlutApp::config.orthoRight, GlutApp::config.orthoTop / 1.7);
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
            // Static road quad
            glColor3ub(50, 50, 50);
            glVertex(roadLeftBottom);
            glVertex2f(roadRightTop.x, roadLeftBottom.y);
            glColor3ub(30, 30, 30);
            glVertex(roadRightTop);
            glVertex2f(roadLeftBottom.x, roadRightTop.y);
            glEnd();
        }
        glEndList();

        glNewList(staticScene + 1, GL_COMPILE);
        glColor(Color::YELLOW);
        for (int i = 0; i < 2; i++) {
            glutBitmapString(font, (const unsigned char *) texts[i].c_str());
            glRasterPos(textsPosition[i]);
            glColor(Color::BLACK);
        }
        glEndList();
    }

    enum AnimationState {
        START, BOTTOM_WEST, NORTH_WEST, TOP_WEST
    };

    AnimationState animState = START;
    Vector2 roadLeftBottom, roadRightTop, roadSize;
    Vector2 textsPosition[2];
    Car staticCar, overcomingCar;
    DemarcationLine demarcation;
    GLuint staticScene = 0;
    const string texts[2]{"\t\t\t\t\t\t\tPress ANY KEY to start", "Overcome (OpenGL 1.0 Demo)"};
    void *font = GLUT_BITMAP_TIMES_ROMAN_24;
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
