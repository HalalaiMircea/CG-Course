#include <array>
#include <cmath>
#include "MyUtils.hpp"
#include "GlutApp.hpp"

class Hw4Pb1 : public AppListener {
    float rotAngle = 0;
    float scaleFactor = 1.0f, scaleDir = -1;
    GLuint triangleListId{};
public:
    void create() override {
        glEnable(GL_POINT_SMOOTH);
        glPointSize(10);
        glLineWidth(6.0);
        glPolygonMode(GL_FRONT, GL_LINE);
        triangleListId = glGenLists(1);
        glNewList(triangleListId, GL_COMPILE);
        {
            glBegin(GL_TRIANGLES);
            glVertex({100, 100}, Color::BLUE);
            glVertex({500, 100}, Color::GREEN);
            glVertex({300, 400}, Color::RED);
            glEnd();
        }
        glEndList();
    }

    void render() override {
        glClearColor(0.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        rotAngle += PI / 3;
        if (scaleFactor <= 0.1f)
            scaleDir = 1;
        else if (scaleFactor >= 1)
            scaleDir = -1;
        scaleFactor += 0.005f * scaleDir;

        // TRIUNGHIUL ORIGINAL
        glCallList(triangleListId);

        // CENTRUL DE GREUTATE
        glBegin(GL_POINTS);
        glVertex({300, 200}, Color::BLACK);
        glEnd();

        // APLICAREA TRANSFORMARII ASUPRA TRIUNGHIULUI
        glPushMatrix();
        glTranslated(300, 200, 0);
        glRotated(rotAngle, 0, 0, 1);
        glScaled(scaleFactor, scaleFactor, 1);
        glTranslated(-300, -200, 0);
        glCallList(triangleListId);
        glPopMatrix();
    }
};

class Hw4Pb2 : public AppListener {
    GLuint polygonListId{};
    float i = 0.0;
    float j = 0.0;
    float alpha = 1.0;
public:
    void create() override {
        std::array<Vector2, 6> vertices;
        Vector2 center(0, 400);
        float size = 75;
        float angle = 0;
        for (int k = 0; k < 6; k++) {
            angle += PI / 3;
            vertices.at(k) = {center.x + size * std::cos(angle), center.y + size * std::sin(angle)};
        }
        glLineWidth(3);

        polygonListId = glGenLists(1);
        glNewList(polygonListId, GL_COMPILE);
        {
            glBegin(GL_POLYGON);
            glVertex(vertices.at(0), Color::RED);
            glVertex(vertices.at(1), {Color::YELLOW});
            glVertex(vertices.at(2), Color::GREEN);
            glVertex(vertices.at(3), Color::CYAN);
            glVertex(vertices.at(4), Color::BLUE);
            glVertex(vertices.at(5), Color::MAGENTA);
            glEnd();
        }
        glEndList();
    }

    void render() override {
        glClearColor(0.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        if (i > (float) GlutApp::config.orthoRight)
            alpha = -1.0;
        else if (i < (float) GlutApp::config.orthoLeft)
            alpha = 1.0;

        i += alpha * 5.0f;
        j += PI / 2;

        glBegin(GL_LINES);
        {
            glColor({0, 0, 0});
            glVertex2i(0, 400);
            glVertex2i(GlutApp::config.orthoRight, 400);
        }
        glEnd();

        glPushMatrix();
        glTranslated(0, 400, 0);
        glTranslated(i, 0, 0);
        glRotated(j, 0, 0, 1);
        glTranslated(0, -400, 0);
        glColor3f(1, 0, 0);
        glCallList(polygonListId);
        glPopMatrix();
    }
};

int main(int argc, char **argv) {
    /*Configuration config1;
    config1.width = 1200;
    config1.height = 800;
    config1.title = "Triunghi rotit in jurul centrului de greutate";
    config1.samples = 8;
    config1.setOrtho(-800.0, 800.0, -600.0, 600.0, -1, 1);

    new GlutApp(new Hw4Pb1(), config1, argc, argv);*/

    Configuration config2;
    config2.width = 1200;
    config2.height = 800;
    config2.title = "Poligon rostogolitor";
    config2.samples = 8;
    config2.setOrtho(0, config2.width, 0, config2.height, -1, 1);

    new GlutApp(new Hw4Pb2(), config2, argc, argv);

    return 0;
}