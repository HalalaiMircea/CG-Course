#include <iostream>
#include <memory>
#include "Car.hpp"
#include "DemarcationLine.hpp"

using namespace std;

class TrafficScene : public AppListener {
    Vector2 roadLeftBottom, roadTopRight;
    Car staticCar, overcomingCar;
    unique_ptr<DemarcationLine> demarcation;
    GLuint staticScene = 0;

public:
    TrafficScene() = default;

    void create() override {
        glutIgnoreKeyRepeat(1);

        generateStaticScene();
        demarcation = make_unique<DemarcationLine>(
                roadLeftBottom.y + (roadTopRight.y - roadLeftBottom.y) / 2 - DemarcationLine::RECT_SIZE.y / 2);

        Car::generateResources();
        staticCar = Car(1600 / 2.0, 800 / 2.0, Color::BLUE);
        overcomingCar = Car(1600, 310, Color::RED);

        staticCar.setSpeed({0, 0});
        overcomingCar.setSpeed({-100, 0});
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
    }

    void keyboardFunc(unsigned char key, int x, int y) override {
        if (key == ' ') {
            /*cout << "Float size: " << sizeof(float) << " bytes" << endl;
            cout << "Vector2 size: " << sizeof(Vector2) << " bytes" << endl;*/
            cout << "All cars size: " << sizeof(staticCar) + sizeof(overcomingCar) << " bytes" << endl;
        } else if (key == 27)
            GlutApp::exit();
    }

private:
    void generateStaticScene() {
        roadLeftBottom = {(float) GlutApp::config.orthoLeft, (float) GlutApp::config.orthoTop / 5};
        roadTopRight = {(float) GlutApp::config.orthoRight, (float) GlutApp::config.orthoTop / 2.25f};

        staticScene = glGenLists(1);
        glNewList(staticScene, GL_COMPILE);
        glBegin(GL_QUADS);
        {
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
        }
        glEnd();
        glColor(Color::BLACK);
        glRect(roadLeftBottom, roadTopRight);
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
