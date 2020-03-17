#include <cmath>
#include "MyUtils.hpp"
#include "GlutApplication.hpp"

//1p) Folositi liste de display (cf. cod sursa 03_L_1_hexagon.cpp) pentru a aproxima cercuri folosind poligoane cu multe
// laturi. Reprezentati cercuri de dimensiuni diferite. Folositi si alte metode de reprezentare, diferite de GL_FILL.
// Desenati centrele cercurilor / o raza si folositi text pentru a le nota / a le scrie valoarea.
class Hw3Problem1 : public AppListener {

    unsigned int displayListId;

    static void drawCircle(float radius, const Vertex &centerVertex) {
        Vertex circleVertex, firstVertex;

        glBegin(GL_POLYGON);
        for (int k = 0; k < 360; k++) {
            float hexTheta = (float) k * degToRad;
            circleVertex.x = centerVertex.x + radius * std::cos(hexTheta);
            circleVertex.y = centerVertex.y + radius * std::sin(hexTheta);
            circleVertex.draw();
            if (k == 0)
                firstVertex = Vertex(circleVertex.x, circleVertex.y);
        }
        glEnd();

        glColor3f(0, 0, 1);
        glBegin(GL_LINES);
        {
            centerVertex.draw();
            firstVertex.draw();
        }
        glEnd();

        glColor3f(0, 0, 0);
        glRasterPos2f(centerVertex.x + radius / 4, centerVertex.y + 10);
        std::string text = "R = " + std::to_string((int) radius);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *) text.c_str());
    }

public:
    void create() override {
        glLineWidth(3);

        displayListId = glGenLists(1);
        glNewList(displayListId, GL_COMPILE);
        {
            Vertex vert = Vertex(150, 150);
            glPolygonMode(GL_FRONT, GL_FILL);
            glColor3f(1.0, 0.0, 0.0);
            drawCircle(75.69, vert);

            vert = Vertex(400, 300);
            glPolygonMode(GL_FRONT, GL_LINE);
            glColor3f(1.0, 0.0, 1.0);
            drawCircle(150, vert);
        }
        glEndList();
    }

    void reshapeFunc(int width, int height) override {
        glViewport(0, 0, GlutApplication::config.width, GlutApplication::config.height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, GlutApplication::config.width, 0, GlutApplication::config.height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void displayFunc() override {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glCallList(displayListId);
    }
};

int main(int argc, char **argv) {
    Configuration config;
    config.width = 800;
    config.height = 600;
    config.title = "Tema 3";
    config.samples = 8;
    config.setOrtho(0, config.width, 0, config.height, -1, 1);

    new GlutApplication(new Hw3Problem1(), config, argc, argv);
    return 0;
}