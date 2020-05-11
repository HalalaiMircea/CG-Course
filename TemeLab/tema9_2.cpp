#include <GL/freeglut.h>
#include <SOIL.h>

unsigned char *image1, *image2;
int textureWidth1, textureHeight1, textureWidth2, textureHeight2;

void loadTexture() {
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);    // Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    image1 = SOIL_load_image("text_smiley_face.png", &textureWidth1, &textureHeight1, 0, SOIL_LOAD_RGB);
    image2 = SOIL_load_image("text_earth.bmp", &textureWidth2, &textureHeight2, 0, SOIL_LOAD_RGB);
}

void init() {
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    loadTexture();
    glEnable(GL_CULL_FACE);
}

static void drawLeftTriangle() {
    /* draw yellow triangle on LHS of screen */

    glBegin(GL_TRIANGLES);
    glColor4f(1.0, 1.0, 0.0, 0.75);
    glVertex2f(0.1, 0.9);
    glVertex2f(0.1, 0.1);
    glVertex2f(0.7, 0.5);
    glEnd();
}

static void drawRightTriangle() {
    /* draw cyan triangle on RHS of screen */

    glBegin(GL_TRIANGLES);
    glColor4f(0.0, 1.0, 1.0, 0.75);
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth1, textureHeight1, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);

    glPushMatrix();
    {
        glScalef(2, 2, 1);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glColor4f(1.0, 0.1, 0.1, 0.9f); glVertex2f(-2, -1);
        glTexCoord2f(1, 1); glColor4f(0.1, 1.0, 0.1, 0.9f); glVertex2f(0, -1);
        glTexCoord2f(1, 0); glColor4f(1.0, 1.0, 0.1, 0.9f); glVertex2f(0, 1);
        glTexCoord2f(0, 0); glColor4f(0.0, 0.1, 1.0, 0.9f); glVertex2f(-2, 1);
        glEnd();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth2, textureHeight2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);

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
        SOIL_free_image_data(image1);
        SOIL_free_image_data(image2);
        glBindTexture(GL_TEXTURE_2D, 0);
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
