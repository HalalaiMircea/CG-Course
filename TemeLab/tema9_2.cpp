#include <GL/freeglut.h>
#include <SOIL.h>

static int leftFirst = GL_TRUE;

void LoadTexture() {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);    // Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;
    unsigned char *image = SOIL_load_image("text_smiley_face.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // SOIL_free_image_data(image);
    // glBindTexture(GL_TEXTURE_2D, 0);

}

void LoadTexture2() {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);    // Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;
    unsigned char *image = SOIL_load_image("text_earth.bmp", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // SOIL_free_image_data(image);
    // glBindTexture(GL_TEXTURE_2D, 0);
}

void init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    glColor3f(0.4, 0.3, 0.1);

    glPushMatrix();
    glTranslatef(0, 2, 0);
    if (leftFirst) {
        drawLeftTriangle();
        drawRightTriangle();
    }
    else {
        drawRightTriangle();
        drawLeftTriangle();
    }
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    LoadTexture();

    glPushMatrix();
    glScalef(2, 2, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 1.0); glColor4f(1.0, 0.1, 0.1,0.9f); glVertex2f(-2.0, -1.0);
    glTexCoord2f(1.0, 0.0); glColor4f(0.1, 1.0, 0.1,0.9f); glVertex2f(-2.0, 1.0);
    glTexCoord2f(0.0, 0.0); glColor4f(1.0, 1.0, 0.1,0.9f); glVertex2f(0.0, 1.0);
    glTexCoord2f(0.0, 1.0); glColor4f(0.0, 0.1, 1.0,0.9f); glVertex2f(0.0, -1.0);
    glEnd();

    LoadTexture2();
    glColor4f(1.0, 1.0, 1.0, 0.7f);
    glPushMatrix();
    glTranslatef(-4, 0, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex2f(2.0, -1.0);
    glTexCoord2f(0.0, 1.0); glVertex2f(2.0, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex2f(4.0, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex2f(4.0, -1.0);
    glEnd();
    glPopMatrix();
    glPopMatrix();

    glFlush();
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
    switch (key) {
        case 27:
            exit(0);
            break;
    }
}

int main(int argc, char **argv) {
    int width = 800, height = 600;
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width), (glutGet(GLUT_SCREEN_HEIGHT) - height));
    glutInitWindowSize(width, height);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
