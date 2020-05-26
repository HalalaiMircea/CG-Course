#include <GL/freeglut.h>
#include <cmath>
#include <map>
#include "MyUtils.hpp"

using namespace std;

float angle = 0.f, fraction = 0.1f;
Vector3 cameraDirection{0.f, 0.f, -1.f};
Vector3 cameraPosition{0.f, 0.f, 5.f};
map<unsigned char, bool> keyStates;
Color groundColor;
GLuint displayLists, currentList;
GLint fogMode;

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
    float groundMaterialCoeff[] = {0.9f, 0.9f, 0.9f, 1.f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, groundMaterialCoeff);
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

    // sursa de lumina
    float pozitial0[] = {0.0, 50, 0.0, 1.0};
    float ambientLight[] = {0.5f, 0.5f, 0.7f, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pozitial0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glFlush();
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

    initialize();

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(processNormalKeys);
    glutKeyboardUpFunc(processNormalUpKeys);

    // enter GLUT event processing cycle
    glutMainLoop();
}

