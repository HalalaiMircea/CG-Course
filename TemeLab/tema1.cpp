#include <GL/freeglut.h>
#include <cmath>
#include <vector>


struct Point {
    int x, y;

    Point(int x, int y) : x(x), y(y) {}
};

const int WIDTH = 800, HEIGHT = 600;
const float PI = 3.14159265358979323846;

std::vector<Point> vertices;
std::vector<int> indices;

float degToRad(float degree) {
    return degree * PI / 180;
}

void calcHexagonVertices(int length, Point center) {
    // We start from 20 degrees and increment 60 every vertex
    float currAngle = degToRad(30);

    vertices.push_back(center);
    for (int i = 1; i <= 6; i++) {
        Point p(length * std::cos(currAngle) + center.x, length * std::sin(currAngle) + center.y);
        vertices.push_back(p);
        currAngle += PI / 3;
    }
    indices = {0, 3, 4, 5, 0, 5, 6, 1, 0, 1, 2, 3};
}

void initialize() { // initializare fereastra de vizualizare
    glClearColor(0, 1.0, 1.0, 1.0); // precizeaza culoarea de fond a ferestrei de vizualizare
    glMatrixMode(GL_PROJECTION);  // se precizeaza este vorba de o reprezentare 2D, realizata prin proiectie ortogonala
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT); // sunt indicate coordonatele extreme ale ferestrei de vizualizare
}

void render() { // procedura desenare
    glPointSize(5);
    glBegin(GL_QUADS);
    for (int p : indices) {
        switch (p) {
            case 0:
                glColor3f(1, 1, 1);
                break;
            case 1:
                glColor3f(0, 1, 1);
                break;
            case 2:
                glColor3f(0, 1, 0);
                break;
            case 3:
                glColor3f(1, 1, 0);
                break;
            case 4:
                glColor3f(1, 0, 0);
                break;
            case 5:
                glColor3f(1, 0, 1);
                break;
            case 6:
                glColor3f(0, 0, 1);
                break;
        }
        glVertex2i(vertices[p].x, vertices[p].y);
    }
    glEnd();

    glFlush(); // proceseaza procedurile OpenGL cat mai rapid
}

int main(int argc, char **argv) {
    glutInit(&argc, argv); // initializare GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100); // pozitia initiala a ferestrei de vizualizare (in coordonate ecran)
    glutInitWindowSize(WIDTH, HEIGHT); // dimensiunile ferestrei
    glutCreateWindow("Hexagon RGB");

    Point p(WIDTH / 2, HEIGHT / 2);
    calcHexagonVertices(200, p);

    initialize(); // executa procedura de initializare
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(render); // procedura desen este invocata ori de cate ori este nevoie
    glutMainLoop(); // ultima instructiune a programului, asteapta (eventuale) noi date de intrare
}