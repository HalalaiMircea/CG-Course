#include "utils/GlutApp3D.hpp"
#include "utils/math/Vector.hpp"
#include "utils/Color.hpp"
#include "utils/glUtils.hpp"
#include "ObjLoader.hpp"
#include <SOIL.h>
#include <map>

using namespace std;

class Application : public AppListener {

    float angle = 0, velocity = 2.5f;
    Vector3 cameraDirection{0.0f, 0.0f, -1.0f};
    Vector3 cameraPosition{0.0f, 1.0f, 5.0f};
    map<unsigned char, bool> keyStates;
    map<int, bool> specialKeyStates;
    Color groundColor{.0f, .5f, .7f};
    GLuint displayList = 0;
    GLuint groundTexture = 0;
    string parentDirectory, filename;

public:
    static void initEnables() {
        glEnable(GL_DEPTH_TEST);
//        glEnable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_FOG);
    }

    void create() override {
        initEnables();

        float lightPosition[] = {0.f, 1.f, 0.f, 0.f};
        float ambientLightColor[] = {.0f, .3f, .4f, 1.f};
        float diffuseLightColor[] = {1.f, 1.f, 1.f, 1.f};
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        // glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambientLightColor);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLightColor);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLightColor);
        // glLightfv(GL_LIGHT0, GL_SPECULAR, diffuseLightColor);

        GLfloat fogColor[4] = {201 / 255.f, 201 / 255.f, 201 / 255.f, 1};
        glFogi(GL_FOG_MODE, GL_EXP2);
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_DENSITY, 0.075f);
        glHint(GL_FOG_HINT, GL_DONT_CARE);
        glFogf(GL_FOG_START, 20.f);
        glFogf(GL_FOG_END, 30.f);

        displayList = ObjLoader::loadModel(parentDirectory, filename);
        groundTexture = SOIL_load_OGL_texture("../assets/tileable-S7002876-verydark.png",
                                              0, false, 0);

        // Configure texture rendering after generating and binding (Loading) textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void render(float delta) override {
        glClearColor(201 / 255.f, 201 / 255.f, 201 / 255.f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Reset transformations
        glLoadIdentity();

        moveCamera();

        // Draw ground
        float groundMaterialColor[4];
        groundColor.toArray(groundMaterialColor);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, groundMaterialColor);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glBegin(GL_QUADS);
        {
            glTexCoord2f(0, 0);
            glNormal3f(.0f, 1.f, .0f);
            glVertex3f(-100.0f, 0.0f, -100.0f);
            glTexCoord2f(0, 20);
            glNormal3f(.0f, 1.f, .0f);
            glVertex3f(-100.0f, 0.0f, 100.0f);
            glTexCoord2f(20, 20);
            glNormal3f(.0f, 1.f, .0f);
            glVertex3f(100.0f, 0.0f, 100.0f);
            glTexCoord2f(20, 0);
            glNormal3f(.0f, 1.f, .0f);
            glVertex3f(100.0f, 0.0f, -100.0f);
        }
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);

        glPushMatrix();
        glRotatef(angle, 0, 1, 0);
        glTranslatef(0, -ObjLoader::lowestVertexY, 0);
        glCallList(displayList);
        glPopMatrix();
    }

    void keyboardDown(unsigned char key, int x, int y) override {
        if (tolower(key) == 'w' || tolower(key) == 'a' || tolower(key) == 's' || tolower(key) == 'd' ||
            tolower(key) == 'q' || tolower(key) == 'e')
            keyStates[tolower(key)] = true;

        if (key == 27)
            GlutApp3D::exit();
    }

    void keyboardUp(unsigned char key, int x, int y) override {
        if (tolower(key) == 'w' || tolower(key) == 'a' || tolower(key) == 's' || tolower(key) == 'd' ||
            tolower(key) == 'q' || tolower(key) == 'e')
            keyStates[tolower(key)] = false;
    }

    void mouseFunc(int button, int state, int x, int y) override {
        if (button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON)
            specialKeyStates[button] = (state == GLUT_DOWN);
    }

    void setModelFullPath(const string &modelFullPath) {
        size_t found;
        found = modelFullPath.find_last_of("/\\");
        parentDirectory = modelFullPath.substr(0, found + 1);
        filename = modelFullPath.substr(found + 1);
    }

private:
    void moveCamera() {
        if (keyStates['a']) {
            angle -= 45.f * GlutApp3D::getDeltaTime();
            //cameraDirection.x = sin(angle);
            //cameraDirection.z = -cos(angle);
        }
        if (keyStates['d']) {
            angle += 45.f * GlutApp3D::getDeltaTime();
            //cameraDirection.x = sin(angle);
            //cameraDirection.z = -cos(angle);
        }
        if (specialKeyStates[GLUT_LEFT_BUTTON]) cameraPosition.y += velocity * GlutApp3D::getDeltaTime();

        if (specialKeyStates[GLUT_RIGHT_BUTTON]) cameraPosition.y -= velocity * GlutApp3D::getDeltaTime();

        // Zoom in
        if (keyStates['w']) cameraPosition.z -= velocity * GlutApp3D::getDeltaTime();

        // Zoom out
        if (keyStates['s']) cameraPosition.z += velocity * GlutApp3D::getDeltaTime();

        gluLookAt(cameraPosition, cameraPosition + cameraDirection, {0, 1, 0});
    }
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " obj_file\n";
        exit(69);
    }

    Configuration config;
    config.width = 1400;
    config.height = 900;
    config.title = "3D Model Viewer";
    config.samples = 8;

    auto *pApplication = new Application();
    pApplication->setModelFullPath(argv[1]);
    GlutApp3D game3D(pApplication, config, argc, argv);
}
