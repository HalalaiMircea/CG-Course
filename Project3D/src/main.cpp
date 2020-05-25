#include "utils/GlutApp3D.hpp"
#include "utils/math/Vector.hpp"
#include "utils/Color.hpp"
#include "utils/glUtils.hpp"
#include "ObjLoader.hpp"
#include <map>

using namespace std;

class TrafficOvertake : public AppListener {
private:
    float angle = 0, velocity = 2.5f;
    Vector3 cameraDirection{0.0f, 0.0f, -1.0f};
    Vector3 cameraPosition{0.0f, 0.0f, 5.0f};
    map<unsigned char, bool> keyStates;
    Color groundColor;
    GLuint displayList = 0;

public:
    void create() override {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        groundColor = Color(0.9f, 0.9f, 0.9f);

        displayList = ObjLoader::loadModel("../assets/Car.obj");

        //displayList = glGenLists(1);
        /*glNewList(displayList, GL_COMPILE);
        {
            glColor3f(1.0f, 1.0f, 1.0f);

            // Draw Body
            float materialCoeffBody[] = {0.0f, 0.7f, 1.f, 1.f};
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialCoeffBody);
            glTranslatef(0.0f, 0.75f, 0.0f);
            glutSolidSphere(0.75f, 50, 50);

            // Draw Head
            glTranslatef(0.0f, 1.0f, 0.0f);
            glutSolidSphere(0.25f, 40, 40);

            // Draw Eyes
            float materialCoeffEyes[] = {0.f, 0.f, 0.f, 1.f};
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialCoeffEyes);
            glPushMatrix();
            glColor3f(0.0f, 0.0f, 0.0f);
            glTranslatef(0.05f, 0.10f, 0.18f);
            glutSolidSphere(0.05f, 30, 30);
            glTranslatef(-0.1f, 0.0f, 0.0f);
            glutSolidSphere(0.05f, 30, 30);
            glPopMatrix();

            // Draw Nose
            float materialCoeffNose[] = {1.0f, 0.4f, 0.0f, 1.f};
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialCoeffNose);
            glColor3f(1.0f, 0.5f, 0.5f);
            glutSolidCone(0.08f, 0.5f, 10, 2);
        }
        glEndList();*/

        glEnable(GL_FOG);
        {
            GLfloat fogColor[4] = {201 / 255.f, 201 / 255.f, 201 / 255.f, 1};

            glFogi(GL_FOG_MODE, GL_EXP2);
            glFogfv(GL_FOG_COLOR, fogColor);
            glFogf(GL_FOG_DENSITY, 0.075f);
            glHint(GL_FOG_HINT, GL_DONT_CARE);
            glFogf(GL_FOG_START, 20.f);
            glFogf(GL_FOG_END, 30.f);
        }
    }

    void render(float delta) override {
        glClearColor(201 / 255.f, 201 / 255.f, 201 / 255.f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Reset transformations
        glLoadIdentity();

        moveCamera();

        // Draw ground
        float groundMaterialCoeff[] = {0.f, 0.9f, 0.f, 1.f};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, groundMaterialCoeff);
        glColor(groundColor);
        glBegin(GL_QUADS);
        glVertex3f(-100.0f, 0.0f, -100.0f);
        glVertex3f(-100.0f, 0.0f, 100.0f);
        glVertex3f(100.0f, 0.0f, 100.0f);
        glVertex3f(100.0f, 0.0f, -100.0f);
        glEnd();

        //glEnable(GL_BLEND);
        // Draw 36 objects
        for (int i = -3; i < 3; i++)
            for (int j = -3; j < 3; j++) {
//            if ((i + j) % 2)
                //glBlendFunc(GL_ONE, GL_ZERO);
                /*else
                    glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_SRC_COLOR);*/
                glPushMatrix();
                glTranslatef(i * 10.0, 0, j * 10.0);
                glCallList(displayList);
                glPopMatrix();
            }
        //glDisable(GL_BLEND);

        // sursa de lumina
        float pozitial0[] = {0.0, 100, 0.0, 1.0};
        float ambientLight[] = {1.f, 1.f, 1.f, 0.0};
        glLightfv(GL_LIGHT0, GL_POSITION, pozitial0);
        glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambientLight);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }

    void keyboardDown(unsigned char key, int x, int y) override {
        if (tolower(key) == 'w' || tolower(key) == 'a' || tolower(key) == 's' || tolower(key) == 'd')
            keyStates[tolower(key)] = true;

        if (key == 27)
            exit(0);
    }

    void keyboardUp(unsigned char key, int x, int y) override {
        if (tolower(key) == 'w' || tolower(key) == 'a' || tolower(key) == 's' || tolower(key) == 'd')
            keyStates[tolower(key)] = false;
    }

private:
    void moveCamera() {
        if (keyStates['a']) {
            angle -= 1.f * GlutApp3D::getDeltaTime();
            cameraDirection.x = sin(angle);
            cameraDirection.z = -cos(angle);
        }
        if (keyStates['d']) {
            angle += 1.f * GlutApp3D::getDeltaTime();
            cameraDirection.x = sin(angle);
            cameraDirection.z = -cos(angle);
        }
        if (keyStates['w'])
            cameraPosition += cameraDirection * velocity * GlutApp3D::getDeltaTime();

        if (keyStates['s'])
            cameraPosition -= cameraDirection * velocity * GlutApp3D::getDeltaTime();

        gluLookAt(cameraPosition.x, 2.0f, cameraPosition.z, cameraPosition.x + cameraDirection.x, 2.0f,
                  cameraPosition.z + cameraDirection.z, 0.0f, 1.0f, 0.0f);
    }
};

int main(int argc, char *argv[]) {
    Configuration config;
    config.width = 1400;
    config.height = 900;
    config.title = "Traffic Overtake";
    config.samples = 8;

    GlutApp3D game3D(new TrafficOvertake(), config, argc, argv);
}
