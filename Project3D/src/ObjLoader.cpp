#include "ObjLoader.hpp"
#include "utils/math/Vector.hpp"
#include "utils/glUtils.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <SOIL.h>


float ObjLoader::lowestVertexY;

GLuint ObjLoader::loadModel(const string &parentDir, const string &filename) {
    using namespace std;
    string line;
    vector<string> tokens;

    vector<Vector3> verts, norms;
    vector<Vector2> texCoords;
    unordered_map<string, vector<Face>> materialFaceMap;
    unordered_map<string, ModelMaterial> materials;
    string currentMaterial = "default";

    ifstream inputStream(parentDir + filename);
    if (!inputStream.is_open()) {
        cerr << "Cannot open OBJ file " << parentDir + filename << '\n';
        exit(69);
    }

    lowestVertexY = numeric_limits<float>::max();

    while (getline(inputStream, line)) {
        tokens = splitString(line, ' ');
        if (tokens.empty()) break;

        if (tokens[0].length() == 0 || tokens[0].at(0) == '#') continue;
        else if (tokens[0] == "v") {
            lowestVertexY = min(stof(tokens[2]), lowestVertexY);
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
            materials = loadMTL(parentDir, tokens[1]);
        } else if (tokens[0] == "usemtl") {
            if (tokens.size() == 1)
                currentMaterial = "default";
            else
                currentMaterial = tokens[1];
        }
    }
    inputStream.close();

    float ambientColor[4], diffuseColor[4], specularColor[4], emissiveColor[4];

    GLuint listId = glGenLists(1);
    glNewList(listId, GL_COMPILE);
    glEnable(GL_BLEND);
    for (auto &mapEntry : materialFaceMap) {
        ModelMaterial &material = materials[mapEntry.first];

        material.ambient.toArray(ambientColor);
        material.diffuse.toArray(diffuseColor);
        material.specular.toArray(specularColor);
        material.emissive.toArray(emissiveColor);

        // For transparency
        ambientColor[3] = diffuseColor[3] = material.opacity;

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
        glMaterialfv(GL_FRONT, GL_EMISSION, emissiveColor);
        glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);

        glColor4fv(diffuseColor);

        glBindTexture(GL_TEXTURE_2D, material.diffTexture);

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
    glDisable(GL_BLEND);
    glEndList();
    return listId;
}

unordered_map<string, ModelMaterial> ObjLoader::loadMTL(const string &parentDir, const string &filename) {
    using namespace std;
    string line;
    vector<string> tokens;

    ModelMaterial currentMat;
    string texFilename;
    unordered_map<string, ModelMaterial> materials;

    ifstream inputStream(filename);
    if (!inputStream.is_open()) {
        cerr << "Cannot open MTL file " + filename << '\n';
        exit(69);
    }

    while (getline(inputStream, line)) {

        if (!line.empty() && line.at(0) == '\t') {
            line = line.substr(1);
            trim(line);
        }

        tokens = splitString(line, ' ');

        if (tokens[0].empty() || tokens[0].at(0) == '#') continue;
        else {
            string key = tokens[0];
            // to lower case
            transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return std::tolower(c); });
            if (key == "newmtl") {
                // Add previously parsed material to the map
                if (!texFilename.empty())
                    currentMat.diffTexture = SOIL_load_OGL_texture(texFilename.c_str(),
                                                                   0, false, SOIL_FLAG_INVERT_Y);
                materials[currentMat.id] = currentMat;

                // Reset attributes to defaults
                currentMat = ModelMaterial();

                if (tokens.size() > 1) currentMat.id = tokens[1];
            } else if (key == "ka" || key == "kd" || key == "ks" || key == "ke") {
                float r = stof(tokens[1]);
                float g = stof(tokens[2]);
                float b = stof(tokens[3]);

                if (key == "ka") currentMat.ambient = Color(r, g, b);
                else if (key == "kd") currentMat.diffuse = Color(r, g, b);
                else if (key == "ks") currentMat.specular = Color(r, g, b);
                else if (key == "ke") currentMat.emissive = Color(r, g, b);

            } else if (key == "tr" || key == "d") currentMat.opacity = stof(tokens[1]);
            else if (key == "ns") currentMat.shininess = stof(tokens[1]);
            else if (key == "map_kd") texFilename = parentDir + tokens[1];
        }
    }
    inputStream.close();
    // Add last material
    materials[currentMat.id] = currentMat;
    return materials;
}

vector<string> ObjLoader::splitString(const string &str, char delimiter) {
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

void ObjLoader::Face::addVertIdx(const string &indexStr) {
    vertIndices.push_back(getIndex(indexStr));
}

void ObjLoader::Face::addTexIdx(const string &indexStr) {
    texIndices.push_back(getIndex(indexStr));
}

void ObjLoader::Face::addNormIdx(const string &indexStr) {
    normIndices.push_back(getIndex(indexStr));
}

int ObjLoader::Face::getIndex(const string &index) {
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

void ObjLoader::ltrim(string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void ObjLoader::rtrim(string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void ObjLoader::trim(string &s) {
    ltrim(s);
    rtrim(s);
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
