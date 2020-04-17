#include <iostream>
#include "Car.hpp"
#include "utils/GlutApp.hpp"

using namespace std;

class TrafficScene : public AppListener {
    Car staticCar{1600 / 2.f, 800 / 2.f, Color::BLUE};
    Car overcomingCar{1600, 310, Color::RED};

public:
    TrafficScene() = default;

    void create() override {
        glutIgnoreKeyRepeat(1);

        staticCar.setSpeed({0, 0});
        overcomingCar.setSpeed({-100, 0});
    }

    void render(float delta) override {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        staticCar.act(delta);
        overcomingCar.act(delta);

        staticCar.draw();
        overcomingCar.draw();
    }

    void keyboardFunc(unsigned char key, int x, int y) override {
        if (key == ' ') {
            /*cout << "Float size: " << sizeof(float) << " bytes" << endl;
            cout << "Vector2 size: " << sizeof(Vector2) << " bytes" << endl;*/
            cout << "All cars size: " << sizeof(staticCar) + sizeof(overcomingCar) << " bytes" << endl;
        }
    }
};

int main(int argc, char *argv[]) {
    Configuration config;
    config.width = 1600;
    config.height = 800;
    config.title = "Traffic Overcome";
    config.samples = 8;
    config.setOrtho(0, 1600, 0, 800, -1, 1);

    auto *glutApp = new GlutApp(new TrafficScene(), config, argc, argv);

    delete glutApp;
    return 0;
}
