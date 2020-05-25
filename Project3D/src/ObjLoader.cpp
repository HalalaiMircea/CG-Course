#include "ObjLoader.hpp"
#include "utils/math/Vector.hpp"
#include "utils/glUtils.hpp"
#include <fstream>
#include <iostream>
#include <unordered_map>

GLuint ObjLoader::loadModel(const std::string &filename) {
    using namespace std;
    string line;
    vector<string> tokens;

    vector<Vector3> verts, norms;
    vector<Vector2> texCoords;
    unordered_map<string, vector<Face>> materialFaceMap;
    string currentMaterial;

    ifstream inputStream(filename);
    if (!inputStream.is_open()) {
        cerr << "Error reading file " + filename << '\n';
        exit(69);
    }

    while (getline(inputStream, line)) {
        tokens = splitString(line, ' ');
        if (tokens.empty()) break;

        if (tokens[0].length() == 0 || tokens[0].at(0) == '#') continue;
        else if (tokens[0] == "v") {
            verts.emplace_back(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
        } else if (tokens[0] == "vn") {
            norms.emplace_back(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
        } else if (tokens[0] == "vt") {
            texCoords.emplace_back(stof(tokens[1]), stof(tokens[2]));
        } else if (tokens[0] == "f") {
            vector<string> parts;
            Face face;
            for (int i = 1; i < tokens.size(); ++i) {
                parts = splitString(tokens[i], '/');
                face.addVertIdx(parts[0]);

                if (parts.size() > 2)
                    face.addNormIdx(parts[2]);

                if (parts.size() > 1 && !parts[1].empty())
                    face.addTexIdx(parts[1]);
            }
            materialFaceMap[currentMaterial].push_back(face); // We add a new face to current material
        } else if (tokens[0] == "mtllib") {
            //TODO load .MTL file
        } else if (tokens[0] == "usemtl") {
            if (tokens.size() == 1)
                currentMaterial = "default";
            else
                currentMaterial = tokens[1];
        }
    }
    inputStream.close();

    GLuint listId = glGenLists(1);
    glNewList(listId, GL_COMPILE);
    {
        for (auto &mapEntry : materialFaceMap) {
            for (auto &face : mapEntry.second) {
                glBegin(GL_POLYGON);
                for (int i = 0; i < face.size(); ++i) {
                    if (face.hasNormals()) glNormal(norms[face.normIndices[i]]);

                    if (face.hasTexCoords()) glTexCoord(texCoords[face.texIndices[i]]);

                    glVertex(verts[face.vertIndices[i]]);
                }
                glEnd();
            }
        }
    }
    glEndList();
    return listId;
}

std::vector<std::string> ObjLoader::splitString(const std::string &str, char delimiter) {
    using namespace std;
    vector<string> tokens;
    size_t current, previous = 0;
    current = str.find(delimiter);
    while (current != string::npos) {
        tokens.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delimiter, previous);
    }
    tokens.push_back(str.substr(previous, current - previous));
    return tokens;
}

void ObjLoader::Face::addVertIdx(const std::string &indexStr) {
    vertIndices.push_back(getIndex(indexStr));
}

void ObjLoader::Face::addTexIdx(const std::string &indexStr) {
    texIndices.push_back(getIndex(indexStr));
}

void ObjLoader::Face::addNormIdx(const std::string &indexStr) {
    normIndices.push_back(getIndex(indexStr));
}

int ObjLoader::Face::getIndex(const std::string &index) {
    if (index.empty() || index.length() == 0) return 0;
    return std::stoi(index) - 1;
}

std::ostream &operator<<(std::ostream &os, const ObjLoader::Face &face) {
    for (int i = 0; i < face.vertIndices.size(); ++i) {
        os << face.vertIndices.at(i);
        if (!face.texIndices.empty() && !face.normIndices.empty())
            os << '/' << face.texIndices.at(i) << '/' << face.normIndices.at(i);
        else if (!face.normIndices.empty())
            os << "//" << face.normIndices.at(i);
        else if (!face.texIndices.empty())
            os << '/' << face.texIndices.at(i);
        os << " ";
    }
    return os;
}

bool ObjLoader::Face::hasTexCoords() const {
    return !texIndices.empty();
}

bool ObjLoader::Face::hasNormals() const {
    return !normIndices.empty();
}

int ObjLoader::Face::size() const {
    return vertIndices.size();
}
