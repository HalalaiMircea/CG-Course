#include "ObjLoader.hpp"
#include "utils/math/Vector.hpp"
#include "utils/glUtils.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

GLuint ObjLoader::loadModel(const std::string &filename) {
    using namespace std;
    string line;
    vector<string> tokens;

    vector<Vector3> verts, norms;
    vector<Vector2> texCoords;
    unordered_map<string, vector<Face>> materialFaceMap;
    unordered_map<string, ModelMaterial> materials;
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
            materials = MtlLoader::load("../assets/" + tokens[1]);
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
    for (auto &mapEntry : materialFaceMap) {
        ModelMaterial &material = materials[mapEntry.first];
        float ambientColor[4], diffuseColor[4], specularColor[4], emissiveColor[4];

        material.ambient.toArray(ambientColor);
        material.diffuse.toArray(diffuseColor);
        material.specular.toArray(specularColor);
        material.emissive.toArray(emissiveColor);

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
        glMaterialfv(GL_FRONT, GL_EMISSION, emissiveColor);
        glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);

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

void ObjLoader::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void ObjLoader::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void ObjLoader::trim(std::string &s) {
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

std::unordered_map<std::string, ModelMaterial> ObjLoader::MtlLoader::load(const std::string &filename) {
    using namespace std;
    unordered_map<string, ModelMaterial> materials;
    string line;
    vector<string> tokens;
    string curMatName = "default";
    Color ambientcolor = Color::WHITE;
    Color difcolor = Color::WHITE;
    Color speccolor = Color::WHITE;
    Color emissivecolor = Color::BLACK;
    float shininess = 0.f;
    //string texFilename;

    ifstream inputStream(filename);
    if (!inputStream.is_open()) {
        cerr << "Error reading file " + filename << '\n';
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
                // Add attributes for the previously parsed material
                ModelMaterial mat;
                mat.id = curMatName;
                mat.ambient = ambientcolor;
                mat.diffuse = difcolor;
                mat.specular = speccolor;
                mat.emissive = emissivecolor;
                mat.shininess = shininess;
                materials[curMatName] = mat;

                if (tokens.size() > 1) {
                    curMatName = tokens[1];
                } else
                    curMatName = "default";

                // Reset attributes to defaults
                ambientcolor = Color::WHITE;
                difcolor = Color::WHITE;
                speccolor = Color::WHITE;
                emissivecolor = Color::BLACK;
                shininess = 0.f;
            } else if (key == "ka" || key == "kd" || key == "ks" || key == "ke") {
                float r = stof(tokens[1]);
                float g = stof(tokens[2]);
                float b = stof(tokens[3]);
                //float a = 1;
                //if (tokens.size() > 4) a = stof(tokens[4]);

                if (key == "ka") ambientcolor = Color(r, g, b);
                else if (key == "kd") difcolor = Color(r, g, b);
                else if (key == "ks") speccolor = Color(r, g, b);
                else if (key == "ke") emissivecolor = Color(r, g, b);

            } else if (key == "ns") shininess = stof(tokens[1]);
        }
    }
    inputStream.close();

    // last material
    ModelMaterial mat;
    mat.id = curMatName;
    mat.ambient = ambientcolor;
    mat.diffuse = difcolor;
    mat.specular = speccolor;
    mat.emissive = emissivecolor;
    mat.shininess = shininess;
    materials[curMatName] = mat;

    return materials;
}