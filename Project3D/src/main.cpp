#include "utils/GlutApp3D.hpp"
#include "utils/math/Vector.hpp"
#include "utils/Color.hpp"
#include "utils/glUtils.hpp"
#include "ObjLoader.hpp"
#include <SOIL.h>
#include <map>

using namespace std;

class Application : public AppListener {
private:
    float angle = 0, velocity = 2.5f;
    Vector3 cameraDirection{0.0f, 0.0f, -1.0f};
    Vector3 cameraPosition{0.0f, 1.0f, 5.0f};
    Vector2 oldMousePos{0, 0};
    map<unsigned char, bool> keyStates;
    map<int, bool> specialKeyStates;
    Color groundColor{.0f, .5f, .7f};
    GLuint displayLists[2]{};
    GLuint groundTexture = 0;
    string parentDirectory, filename;

public:
    void create() override {
        glEnable(GL_DEPTH_TEST);
        configureLighting();
        configureFog();
        configureGLUTMenus();

        groundTexture = SOIL_load_OGL_texture("../assets/tileable-S7002876-verydark.png", 0, false, SOIL_FLAG_INVERT_Y);
        configureLoadedTexture();

        displayLists[0] = glGenLists(1);
        glNewList(displayLists[0], GL_COMPILE);
            float groundMaterialColor[4];
            groundColor.toArray(groundMaterialColor);
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, groundMaterialColor);
            glBindTexture(GL_TEXTURE_2D, groundTexture);
            glBegin(GL_QUADS);

            glNormal3f(.0f, 1.f, .0f);
            glTexCoord2f(0, 0);glVertex3f(-100.0f, 0.0f, -100.0f);
            glTexCoord2f(0, 20);glVertex3f(-100.0f, 0.0f, 100.0f);
            glTexCoord2f(20, 20);glVertex3f(100.0f, 0.0f, 100.0f);
            glTexCoord2f(20, 0);glVertex3f(100.0f, 0.0f, -100.0f);

            glEnd();
        glEndList();
        displayLists[1] = ObjLoader::loadModel(parentDirectory, filename);
    }

    void render(float delta) override {
        glClearColor(201 / 255.f, 201 / 255.f, 201 / 255.f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity(); // Reset camera transformations
        moveCamera();

        glCallList(displayLists[0]);

        glPushMatrix();
            glRotatef(angle, 0, 1, 0);
            glTranslatef(0, -ObjLoader::lowestVertexY, 0);
            glCallList(displayLists[1]);
        glPopMatrix();
    }

    void keyboardDown(unsigned char key, int x, int y) override {
        if (tolower(key) == 'w' || tolower(key) == 'a' || tolower(key) == 's' || tolower(key) == 'd')
            keyStates[tolower(key)] = true;

        if (key == 27) GlutApp3D::exit();
    }

    void keyboardUp(unsigned char key, int x, int y) override {
        if (tolower(key) == 'w' || tolower(key) == 'a' || tolower(key) == 's' || tolower(key) == 'd')
            keyStates[tolower(key)] = false;
    }

    void mouseFunc(int button, int state, int x, int y) override {
        if (button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON)
            specialKeyStates[button] = (state == GLUT_DOWN);

        // Wheel reports as button 3(scroll up) and button 4(scroll down)
        if (button == 3 || button == 4) {
            if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events

            if (button == 3 && cameraPosition.z > 0)
                cameraPosition.z -= 0.5f;
            if (button == 4 && cameraPosition.z < 10)
                cameraPosition.z += 0.5f;
        }
    }

    void mouseMotion(int mouseX, int mouseY) override {
        Vector2 mouseDistance = Vector2((float) mouseX, (float) mouseY) - oldMousePos;

        if (specialKeyStates[GLUT_LEFT_BUTTON]) { angle += 0.5f * mouseDistance.x; }
        if (specialKeyStates[GLUT_RIGHT_BUTTON]) {
            // When mouse is moved up, the distance is negative
            if (mouseDistance.y < 0) {
                if (cameraPosition.y > 0.2f)
                    cameraPosition.y -= 0.01f * abs(mouseDistance.y);
            } else {
                if (cameraPosition.y < 6.5f)
                    cameraPosition.y += 0.01f * abs(mouseDistance.y);
            }
            cameraPosition.x -= 0.01f * mouseDistance.x;
        }
        oldMousePos.set((float) mouseX, (float) mouseY);
    }

    void mousePassiveMotion(int mouseX, int mouseY) override {
        oldMousePos.set((float) mouseX, (float) mouseY);
    }

    void setModelFullPath(const string &modelFullPath) {
        size_t found = modelFullPath.find_last_of("/\\");
        parentDirectory = modelFullPath.substr(0, found + 1);
        filename = modelFullPath.substr(found + 1);
    }

private:
    void moveCamera() {
        if (keyStates['a']) { angle -= 45.f * GlutApp3D::getDeltaTime(); }
        if (keyStates['d']) { angle += 45.f * GlutApp3D::getDeltaTime(); }
        // Zoom in
        if (keyStates['w'] && cameraPosition.z > 0) { cameraPosition.z -= velocity * GlutApp3D::getDeltaTime(); }
        // Zoom out
        if (keyStates['s'] && cameraPosition.z < 10) { cameraPosition.z += velocity * GlutApp3D::getDeltaTime(); }

        gluLookAt(cameraPosition, cameraPosition + cameraDirection, {0, 1, 0});
    }

    enum RenderMode {
        NORMAL, TRANSPARENT_COLOR, TRANSPARENT_WHITE
    };

    static void menu(RenderMode renderMode) {
        switch (renderMode) {
            case RenderMode::NORMAL:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glDisable(GL_CULL_FACE);
                break;
            case RenderMode::TRANSPARENT_COLOR:
                glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_SRC_COLOR);
                glEnable(GL_CULL_FACE);
                break;
            case RenderMode::TRANSPARENT_WHITE:
                glBlendFunc(GL_ONE, GL_ONE);
                glEnable(GL_CULL_FACE);
                break;
        }
    }

    static void configureLighting() {
        glEnable(GL_LIGHTING);

        float intensity[] = {.1f, .1f, .1f, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, intensity);

        glEnable(GL_LIGHT0);
        float light0Position[] = {1, 1, 0, 0};
        float ambientLightColor[] = {.1f, .1f, .1f, 1.f};
        float diffuseLightColor[] = {1.f, 1.f, 1.f, 1.f};
        glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLightColor);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLightColor);

        glEnable(GL_LIGHT1);
        float light1Position[] = {-1, 1, 0, 0};
        glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
        glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLightColor);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLightColor);
    }

    static void configureFog() {
        glEnable(GL_FOG);

        float fogColor[4] = {201 / 255.f, 201 / 255.f, 201 / 255.f, 1};
        glFogi(GL_FOG_MODE, GL_EXP2);
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_DENSITY, 0.075f);
        glHint(GL_FOG_HINT, GL_DONT_CARE);
        glFogf(GL_FOG_START, 20.f);
        glFogf(GL_FOG_END, 30.f);
    }

    static void configureGLUTMenus() {
        glutCreateMenu(reinterpret_cast<void (*)(int)>(menu));
        glutAddMenuEntry("Normal", RenderMode::NORMAL);
        glutAddMenuEntry("Transparent Color", RenderMode::TRANSPARENT_COLOR);
        glutAddMenuEntry("Transparent White", RenderMode::TRANSPARENT_WHITE);
        glutAttachMenu(GLUT_MIDDLE_BUTTON);
        menu(RenderMode::NORMAL);
    }

    static void configureLoadedTexture() {
        glEnable(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " obj_file\n";
        exit(69);
    } else if (string(argv[1]).substr(string(argv[1]).find_last_of('.') + 1) != "obj") {
        cerr << "File given as argument doesn't have .obj extension\n";
        exit(70);
    }

    Configuration config;
    config.width = 1400;
    config.height = 900;
    config.title = "3D Model Viewer";
    config.samples = 8;
    config.fullscreen = false;

    auto *pApplication = new Application();
    pApplication->setModelFullPath(argv[1]);
    GlutApp3D game3D(pApplication, config, argc, argv);
}