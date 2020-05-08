#include <GL/freeglut.h>

GLint winWidth = 1000, winHeight = 1000;
GLdouble xx0 = 6.0, yy0 = 9.0, zz0 = 10.0;
GLdouble xref = 0.0, yref = 0.0, zref = 0.0;
GLfloat Vx = 1.0, Vy = -1.0, Vz = 0.0;

GLfloat xwMin = -30.0, ywMin = -30.0, xwMax = 30.0, ywMax = 30.0;

GLfloat dnear = 1.0, dfar = 40.0;

static float theta;

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0, 0, 0);

    glMatrixMode(GL_MODELVIEW);
    gluLookAt(xx0, yy0, zz0, xref, yref, zref, Vx, Vy, Vz);

    glMatrixMode(GL_PROJECTION);
    glOrtho(xwMin, xwMax, ywMin, ywMax, dnear, dfar);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void desen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLineWidth(1.0);

    glPushMatrix();
    {
        // roteste corpurile geometrice
        glRotatef(theta, 1, 0, 0);

        //cub albastru
        float diffuseCoeffb[] = {0, 0, 1, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, diffuseCoeffb);
        glutSolidCube(6.0);

        // cub violet mic
        float diffuseCoeffv[] = {1, 0, 1, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, diffuseCoeffv);

        glPushMatrix();
        glTranslatef(0.0, 0.0, 8.0);
        glutSolidCube(2.0);
        glPopMatrix();

        //con rosu
        float diffuseCoeffr[] = {1, 0, 0, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, diffuseCoeffr);

        glPushMatrix();
        glTranslatef(5.0, 3.0, 2.0);
        glutWireCone(6.0, 5.0, 150, 150);
        glPopMatrix();

        //sfera verde
        float diffuseCoeffve[] = {0, 1, 0, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, diffuseCoeffve);
        glutWireSphere(4.0, 50, 50);
    }
    glPopMatrix();    // axele si sursa de lumina nu se rotesc

    float diffuseCoeffn[] = {0, 0, 0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, diffuseCoeffn);

    glLineWidth(1.5);
    //axele de coordonate
    glBegin(GL_LINES);
    glVertex3i(0, 0, 0);
    glVertex3i(20, 0, 0);
    glVertex3i(0, 0, 0);
    glVertex3i(0, 20, 0);
    glVertex3i(0, 0, 0);
    glVertex3i(0, 0, 19);
    glEnd();
    //literele
    glRasterPos3i(21, 0, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'x');
    glRasterPos3i(0, 21, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'y');
    glRasterPos3i(0, 0, 20);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'z');

    // sursa de lumina
    GLfloat pozitial0[] = {0.0, 5.0, 0.0, 1.0};
    GLfloat alb[] = {0.5, 0.5, 0.5, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pozitial0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, alb);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glFlush();
    glutSwapBuffers();
}

void reshapeFcn(GLint newWidth, GLint newHeight) {
    glViewport(0, 0, newWidth, newHeight);

    winWidth = newWidth;
    winHeight = newHeight;
}

void mutarep() {
    theta += 1.0f;
    glutPostRedisplay();
}

void mutarem() {
    theta -= 1.0f;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) glutIdleFunc(mutarep);
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) glutIdleFunc(mutarem);
            break;
        default:
            break;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - winWidth) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - winHeight) / 2);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Efecte de iluminare");
    init();
    glutDisplayFunc(desen);
    glutIdleFunc(desen);
    glutReshapeFunc(reshapeFcn);
    glutMouseFunc(mouse);
    glutMainLoop();
}