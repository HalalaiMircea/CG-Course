#include <cmath>
#include <iostream>
#include <vector>
#include "MyUtils.hpp"
#include "GlutApplication.hpp"

//1. (1p) Folositi liste de display (cf. cod sursa 03_L_1_hexagon.cpp) pentru a aproxima cercuri folosind poligoane cu multe
// laturi. Reprezentati cercuri de dimensiuni diferite. Folositi si alte metode de reprezentare, diferite de GL_FILL.
// Desenati centrele cercurilor / o raza si folositi text pentru a le nota / a le scrie valoarea.
class Hw3Pb1 : public AppListener {

    unsigned int displayListId{};

    static void drawCircle(float radius, const Vector2 &center) {
        Vector2 circleVertex, firstVertex;

        glBegin(GL_POLYGON);
        for (int k = 0; k < 360; k++) {
            float angle = (float) k * degToRad;
            circleVertex.x = center.x + radius * std::cos(angle);
            circleVertex.y = center.y + radius * std::sin(angle);
            glVertex(circleVertex);
            if (k == 0)
                firstVertex = Vector2(circleVertex.x, circleVertex.y);
        }
        glEnd();

        glColor3f(0, 0, 1);
        glBegin(GL_LINES);
        {
            glVertex(center);
            glVertex(firstVertex);
        }
        glEnd();

        glColor3f(0, 0, 0);
        glRasterPos2f(center.x + radius / 4, center.y + 10);
        std::string text = "R = " + std::to_string((int) radius);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *) text.c_str());
    }

public:
    void create() override {
        glLineWidth(5);

        displayListId = glGenLists(1);
        glNewList(displayListId, GL_COMPILE);
        {
            Vector2 vert = Vector2(150, 150);
            glPolygonMode(GL_FRONT, GL_FILL);
            glColor3f(1.0, 1.0, 0.0);
            drawCircle(75.69, vert);

            vert = Vector2(400, 300);
            glPolygonMode(GL_FRONT, GL_LINE);
            glColor3f(1.0, 0.0, 0.0);
            drawCircle(150, vert);
        }
        glEndList();
    }

    void displayFunc() override {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glCallList(displayListId);
    }

    void reshapeFunc(int width, int height) override {
        glViewport(0, 0, GlutApplication::config.width, GlutApplication::config.height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, GlutApplication::config.width, 0, GlutApplication::config.height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
};

//2. a) 0.5p determinati un punct M in interiorul sau pe laturile triunghiului;
class Hw3Pb2a : public AppListener {
    std::vector<Vector2> vertices;
    Vector2 pointM;
public:

    void create() override {
        pointM = Vector2();
        glPointSize(10);
    }

    void mouseFunc(int button, int state, int x, int y) override {
        auto xf = (float) x;
        auto yf = (float) (GlutApplication::config.height - y);

        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (vertices.size() < 3) {
                vertices.emplace_back(xf, yf);
                std::cout << vertices.back() << "\n";
            } else {
                pointM = Vector2(xf, yf);
                float areaABC = triangleArea(vertices.at(0), vertices.at(1), vertices.at(2)),
                        areaMAB = triangleArea(pointM, vertices.at(0), vertices.at(1)),
                        areaMBC = triangleArea(pointM, vertices.at(1), vertices.at(2)),
                        areaMCA = triangleArea(pointM, vertices.at(2), vertices.at(0));
                std::cout << "Punctul M(" << pointM << ") ";
                if (areaABC == areaMAB + areaMBC + areaMCA)
                    std::cout << "e in triunghi\n";
                else
                    std::cout << "e in exterior\n";
            }
        }
    }

    void displayFunc() override {
        glClearColor(0.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        for (const Vector2 &vert : vertices) {
            glColor3f(1.0f, 0.0f, 0.5f);
            glVertex(vert);
        }
        glEnd();

        glBegin(GL_POINTS);
        glColor3f(0, 1, 0);
        glVertex(pointM);
        glEnd();
    }

    void reshapeFunc(int width, int height) override {
        glViewport(0, 0, GlutApplication::config.width, GlutApplication::config.height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, GlutApplication::config.width, 0, GlutApplication::config.height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

private:
    static float triangleArea(const Vector2 &v0, const Vector2 &v1, const Vector2 &v2) {
        return abs(v0.x * v1.y + v2.x * v0.y + v1.x * v2.y - v2.x * v1.y - v0.x * v2.y - v1.x * v0.y) / 2;
    }
};

//2. b) 0.5p deteminati D in planul triunghiului asa incat ABDC (atentie la ordine!!!) sa fie un patrulater convex
class Hw3Pb2b : public AppListener {
    std::vector<Vector2> vertices;
public:
    void create() override {
        glPointSize(15);
    }

    void mouseFunc(int button, int state, int x, int y) override {
        auto xf = (float) x;
        auto yf = (float) (GlutApplication::config.height - y);

        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (vertices.size() < 3) {
                vertices.emplace_back(xf, yf);
                std::cout << vertices.back() << "\n";
            }
            if (vertices.size() == 3) {
                Vector2 newVec(vertices[2]);
                newVec += vertices[1];
                newVec -= vertices[0];
                vertices.emplace_back(newVec);
                std::cout << vertices.back() << "\n";
            }
        }
    }

    void displayFunc() override {
        glClearColor(0.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        if (vertices.size() == 4) {
            glBegin(GL_QUADS);
            glColor(Color::RED);
            glVertex(vertices.at(0));
            glVertex(vertices.at(1));
            glVertex(vertices.at(3));
            glVertex(vertices.at(2));
            glEnd();
        }

        glBegin(GL_POINTS);
        for (int i = 0; i < vertices.size(); ++i)
            if (i == 3) {
                glColor(Color::GREEN);
                glVertex(vertices.at(3));
            } else {
                glColor3f(0.5, 0.5, 1);
                glVertex(vertices.at(i));
            }
        glEnd();
    }

    void reshapeFunc(int width, int height) override {
        glViewport(0, 0, GlutApplication::config.width, GlutApplication::config.height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, GlutApplication::config.width, 0, GlutApplication::config.height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
};

int main(int argc, char **argv) {
    /*Configuration config;
    config.width = 800;
    config.height = 600;
    config.title = "Tema 3: Arie triunghi";
    config.samples = 8;
    config.setOrtho(0, config.width, 0, config.height, -1, 1);

    new GlutApplication(new Hw3Pb2b(), config, argc, argv);*/
    Vector3 v1(1, 0, 0), v2(0, 1, 0);
    std::cout << Vector3::cross(v1, v2) << "\n";
    std::cout << v1 << "\n";
    std::cout << v2 << "\n";
    return 0;
}