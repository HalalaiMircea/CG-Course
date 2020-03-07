#include "MyUtils.hpp"
#include <GL/freeglut.h>
#include <array>

const int WND_WIDTH = 800, WND_HEIGHT = 600;

std::array<Vertex, 4> vertices = {Vertex(370, 30), {20, 380}, {10, 10}, {500, 500}};
std::array<Color, 4> colors = {Color(1, 0, 0), {1, 0, 0}, {0, 1, 0}, {0, 1, 0}};

void init() {
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WND_WIDTH, 0.0, WND_HEIGHT);
}

void render() {
    glClearColor(0, 1, 1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);


    glLineWidth(3.0);
    glBegin(GL_LINES);
    /*glColor3f(1.0, 0.0, 0.0);
    glVertex2i(370, 30);
    glVertex2i(20, 380);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2i(10, 10);
    glVertex2i(500, 500);*/
    for (int i = 0; i < vertices.size(); i++)
        vertices[i].draw(colors[i]);
    glEnd();


    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WND_WIDTH, WND_HEIGHT);
    glutCreateWindow("Template window name");

    init();
    glutDisplayFunc(render);
    glutMainLoop();
}
