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
        createStaticSceneDisplayList();

        Car::generateResources();
        staticCar = Car(roadLeftBottom.x + (roadSize.x - Car::getModelWidth()) / 2,
                        roadRightTop.y - (roadSize.y - Car::getModelHeight()) / 2, Color::BLUE);
        overcomingCar = Car(roadSize.x + 100, roadLeftBottom.y + roadSize.y / 4, Color::RED);
    }

    void render(float delta) override {
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update logic
        demarcation.act(delta);
        staticCar.act(delta);
        overcomingCar.act(delta);

        // Draw objects
        glCallList(staticScene);
        demarcation.draw();
        staticCar.draw();
        overcomingCar.draw();
        if (isMessageDisplayed) {
            for (int i = 0; i < 2; i++) {
                glutBitmapString(font, (const unsigned char *) texts[i].c_str());
                glColor(Color::BLACK);
                glRasterPos(textsPosition[i]);
            }
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
    void initializeDemarcationLine() {
        roadLeftBottom = Vector2((float) GlutApp::config.orthoLeft, (float) GlutApp::config.orthoTop / 5);
        roadRightTop = Vector2((float) GlutApp::config.orthoRight, (float) GlutApp::config.orthoTop / 2.25f);
        roadSize = roadRightTop - roadLeftBottom;

        demarcation.setY(roadLeftBottom.y + (roadSize.y - DemarcationLine::RECT_SIZE.y) / 2);
    }

    void initializeTextsPositions() {
        for (int i = 0; i < 2; ++i) {
            float textX = ((float) GlutApp::config.orthoRight -
                           (float) glutBitmapLength(font, (const unsigned char *) texts[i].c_str())) / 2;
            float textY = (float) GlutApp::config.orthoTop / 1.25f - (float) (glutBitmapHeight(font) * i * 5);
            textsPosition[i] = Vector2(textX, textY);
        }
    }

    void createStaticSceneDisplayList() {
        staticScene = glGenLists(1);
        glNewList(staticScene, GL_COMPILE);
        {
            glBegin(GL_QUADS);
            // Green field quad
            glColor3f(0.604, 0.804, 0.196);
            glVertex2f(0, 0);
            glVertex2f(GlutApp::config.orthoRight, 0);
            glColor3f(0, 0.392, 0);
            glVertex2f(GlutApp::config.orthoRight, GlutApp::config.orthoTop / 1.7);
            glVertex2f(0, GlutApp::config.orthoTop / 1.7);
            // Sky quad
            glColor3ub(200, 240, 255);
            glVertex2f(0, GlutApp::config.orthoTop / 1.7);
            glVertex2f(GlutApp::config.orthoRight, GlutApp::config.orthoTop / 1.7);
            glColor3ub(0, 183, 255);
            glVertex2f(GlutApp::config.orthoRight, GlutApp::config.orthoTop);
            glVertex2f(0, GlutApp::config.orthoTop);
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
    }

    Vector2 roadLeftBottom, roadRightTop, roadSize;
    Vector2 textsPosition[2];
    Car staticCar, overcomingCar;
    DemarcationLine demarcation;
    GLuint staticScene = 0;
    bool isMessageDisplayed = true;
    const string texts[2]{"\t\t\t\t\tPress SPACEBAR to start", "Overcome (OpenGL 1.0 Demo)"};
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
