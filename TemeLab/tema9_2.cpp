#include <GL/freeglut.h>
#include <SOIL.h>

GLuint textureIDs[2];

void loadTextures() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    textureIDs[0] = SOIL_load_OGL_texture("text_smiley_face.png", 0, false, 0);
    textureIDs[1] = SOIL_load_OGL_texture("text_earth.bmp", 0, false, 0);
}

void init() {
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    loadTextures();
}

static void drawLeftTriangle() {
    /* draw yellow triangle on LHS of screen */

    glColor4f(1.0, 1.0, 0.0, 0.75);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.1, 0.9);
    glVertex2f(0.1, 0.1);
    glVertex2f(0.7, 0.5);
    glEnd();
}

static void drawRightTriangle() {
    /* draw cyan triangle on RHS of screen */

    glColor4f(0.0, 1.0, 1.0, 0.75);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.9, 0.9);
    glVertex2f(0.3, 0.5);
    glVertex2f(0.9, 0.1);
    glEnd();
}

void display() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    {
        glColor3f(0.4, 0.3, 0.1);
        glTranslatef(0, 2, 0);
        drawLeftTriangle();
        drawRightTriangle();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glBindTexture(GL_TEXTURE_2D, textureIDs[0]);

        glScalef(2, 2, 1);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glColor4f(1.0, 0.1, 0.1, 0.9f); glVertex2f(-2, -1);
        glTexCoord2f(1, 1); glColor4f(0.1, 1.0, 0.1, 0.9f); glVertex2f(0, -1);
        glTexCoord2f(1, 0); glColor4f(1.0, 1.0, 0.1, 0.9f); glVertex2f(0, 1);
        glTexCoord2f(0, 0); glColor4f(0.0, 0.1, 1.0, 0.9f); glVertex2f(-2, 1);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, textureIDs[1]);

        glColor4f(1.0, 1.0, 1.0, 0.7f);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(-2, -1);
        glTexCoord2f(1, 1); glVertex2f(0, -1);
        glTexCoord2f(1, 0); glVertex2f(0, 1);
        glTexCoord2f(0, 0); glVertex2f(-2, 1);
        glEnd();
    }
    glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        gluOrtho2D(-4.0, 4.0, -4.0 * (GLfloat) h / (GLfloat) w,
                   4.0 * (GLfloat) h / (GLfloat) w);
    else
        gluOrtho2D(-4.0 * (GLfloat) w / (GLfloat) h,
                   4.0 * (GLfloat) w / (GLfloat) h, -4.0, 4.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(2, textureIDs);
        exit(0);
    }
}

int main(int argc, char **argv) {
    int width = 800, height = 600;
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width)/2, (glutGet(GLUT_SCREEN_HEIGHT) - height)/2);
    glutInitWindowSize(width, height);
    glutCreateWindow("Exemplu Texturi");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
