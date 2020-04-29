#include <GL/freeglut.h>
#include <cmath>
#include <map>
#include "MyUtils.hpp"

using namespace std;

float angle = 0.f;
Vector3 cameraDirection{0.f, 0.f, -1.f};
Vector3 cameraPosition{0.f, 0.f, 5.f};
float fraction = 0.1f;
GLuint displayLists, currentList;
map<unsigned char, bool> keyStates;
Color groundColor;

enum class Season {
    WINTER, SPRING
} season;

void changeSize(int w, int h) {
    float ratio = (float) w / (float) h;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(70.0f, ratio, 0.1f, 100.0f);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void menu(Season selectedSeason) {
    season = selectedSeason;
    if (season == Season::WINTER) {
        glClearColor(0, 0, 0, 1);
        groundColor = Color(0.9f, 0.9f, 0.9f);
        currentList = displayLists;
    } else {
        glClearColor(150 / 255.f, 207 / 255.f, 234 / 255.f, 1);
        groundColor = Color(0, .65f, 0);
        currentList = displayLists + 1;
    }
}

void initialize() {
    // OpenGL initialize
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutSetKeyRepeat(0);
    glutCreateMenu(reinterpret_cast<void (*)(int)>(menu));
    glutAddMenuEntry("Winter", static_cast<int>(Season::WINTER));
    glutAddMenuEntry("Spring", static_cast<int>(Season::SPRING));
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    displayLists = glGenLists(2);

    // Snowman display list
    glNewList(displayLists, GL_COMPILE);
    {
        glColor3f(1.0f, 1.0f, 1.0f);

        // Draw Body
        glTranslatef(0.0f, 0.75f, 0.0f);
        glutSolidSphere(0.75f, 50, 50);

        // Draw Head
        glTranslatef(0.0f, 1.0f, 0.0f);
        glutSolidSphere(0.25f, 40, 40);

        // Draw Eyes
        glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);
        glTranslatef(0.05f, 0.10f, 0.18f);
        glutSolidSphere(0.05f, 30, 30);
        glTranslatef(-0.1f, 0.0f, 0.0f);
        glutSolidSphere(0.05f, 30, 30);
        glPopMatrix();

        // Draw Nose
        glColor3f(1.0f, 0.5f, 0.5f);
        glutSolidCone(0.08f, 0.5f, 10, 2);
    }
    glEndList();

    // Spring tree display list
    glNewList(displayLists + 1, GL_COMPILE);
    {
        // Draw cool bush
        glPushMatrix();
        glTranslatef(1.f, 0, .7f);
        glColor3ub(0, 100, 0);
        glutSolidSphere(.4f, 20, 20);
        glPopMatrix();

        // Draw magenta flower
        glPushMatrix();
        glTranslatef(-.5f, 0, -.2f);
        glColor(Color::MAGENTA);
        glutSolidSphere(.15f, 20, 20);
        glPopMatrix();

        // Draw tree trunk
        GLUquadric *obj = gluNewQuadric();
        glColor3f(0.64f, 0.16f, 0.16f);
        glPushMatrix();
        glRotatef(-90, 1.0, 0.0, 0.0);
        gluCylinder(obj, .2f, .1f, 2.f, 30, 10);
        glPopMatrix();

        float sphereRad = 0.5f;
        // Draw tree crown
        glTranslatef(-0.25f, 2.0f, 0.0f);
        glColor3f(0.0f, .9f, 0.0f);
        glutSolidSphere(sphereRad, 20, 20);

        glTranslatef(-0.3f, 0.1f, 0.0f);
        glColor3f(0.0f, 0.85f, 0.0f);
        glutSolidSphere(sphereRad, 20, 20);

        glTranslatef(0.75f, -0.1f, 0.0f);
        glColor3f(0.0f, 0.8f, 0.0f);
        glutSolidSphere(sphereRad, 20, 20);

        glTranslatef(0.3f, 0.1f, 0.0f);
        glColor3f(0.0f, 0.9f, 0.0f);
        glutSolidSphere(sphereRad, 20, 20);

        glTranslatef(-0.25f, 0.2f, 0.0f);
        glColor3f(0.0f, 0.7f, 0.0f);
        glutSolidSphere(sphereRad, 20, 20);

        glTranslatef(-0.3f, 0.f, 0.0f);
        glColor3f(0.0f, 0.85f, 0.0f);
        glutSolidSphere(sphereRad, 20, 20);

        glTranslatef(-0.3f, 0.f, 0.0f);
        glColor3f(0.0f, 0.75f, 0.0f);
        glutSolidSphere(sphereRad, 20, 20);

        glTranslatef(0.1f, 0.2f, 0.0f);
        glColor3f(0.0f, 0.95f, 0.0f);
        glutSolidSphere(sphereRad, 20, 20);

        glTranslatef(0.2f, 0.f, 0.0f);
        glColor3f(0.0f, 0.65f, 0.0f);
        glutSolidSphere(sphereRad, 20, 20);

        glTranslatef(0.3f, 0.f, 0.0f);
        glColor3f(0.0f, 0.55f, 0.0f);
        glutSolidSphere(sphereRad, 20, 20);
    }
    glEndList();

    menu(Season::WINTER);
}

void moveCamera() {
    if (keyStates['a']) {
        angle -= 0.01f;
        cameraDirection.x = sin(angle);
        cameraDirection.z = -cos(angle);
    }
    if (keyStates['d']) {
        angle += 0.01f;
        cameraDirection.x = sin(angle);
        cameraDirection.z = -cos(angle);
    }
    if (keyStates['w'])
        cameraPosition += cameraDirection * Vector3(fraction, 1, fraction);

    if (keyStates['s'])
        cameraPosition -= cameraDirection * Vector3(fraction, 1, fraction);

    gluLookAt(cameraPosition.x, 1.0f, cameraPosition.z, cameraPosition.x + cameraDirection.x, 1.0f,
              cameraPosition.z + cameraDirection.z, 0.0f, 1.0f, 0.0f);
}

void renderScene() {
    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Reset transformations
    glLoadIdentity();

    // Set the camera
    moveCamera();

    // Draw ground
    glColor(groundColor);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();

    // Draw 36 objects
    for (int i = -3; i < 3; i++)
        for (int j = -3; j < 3; j++) {
            glPushMatrix();
            glTranslatef(i * 10.0, 0, j * 10.0);
            glCallList(currentList);
            glPopMatrix();
        }

    glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {
    keyStates[tolower(key)] = true;
    if (key == 27)
        exit(0);
}

void processNormalUpKeys(unsigned char key, int x, int y) {
    keyStates[tolower(key)] = false;
}

int main(int argc, char **argv) {
    // initialize GLUT and create window
    int width = 1280, height = 720;
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2);
    glutInitWindowSize(width, height);
    glutCreateWindow("Scena 3D cu oameni de zapada");

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(processNormalKeys);
    glutKeyboardUpFunc(processNormalUpKeys);

    initialize();

    // enter GLUT event processing cycle
    glutMainLoop();
}

