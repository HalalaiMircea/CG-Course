#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <ostream>
#include <GL/gl.h>
#include "ModelMaterial.hpp"

using std::string;
using std::unordered_map;
using std::vector;

class ObjLoader {
public:
    static float lowestVertexY;

    static GLuint loadModel(const string &parentDir, const string &filename);

private:
    static unordered_map<string, ModelMaterial> loadMTL(const string &parentDir, const string &filename);
    static vector<string> splitString(const string &str, char delimiter);
    static void ltrim(string &s);
    static void rtrim(string &s);
    static void trim(string &s);

public:
    class Face {
    public:
        vector<int> vertIndices;
        vector<int> texIndices;
        vector<int> normIndices;

        void addVertIdx(const string &indexStr);
        void addTexIdx(const string &indexStr);
        void addNormIdx(const string &indexStr);
        bool hasTexCoords() const;
        bool hasNormals() const;
        int size() const;

    private:
        static int getIndex(const string &index);
        friend std::ostream &operator<<(std::ostream &os, const Face &face);
    };

private:
    static void printLoadedModelSize(const unordered_map<string, vector<Face>>& data);
};