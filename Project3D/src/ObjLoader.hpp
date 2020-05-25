#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <ostream>
#include <GL/gl.h>
#include "ModelMaterial.hpp"

class ObjLoader {
public:
    static GLuint loadModel(const std::string &filename);

private:
    static std::vector<std::string> splitString(const std::string &str, char delimiter);

    static void ltrim(std::string &s);

    static void rtrim(std::string &s);

    static void trim(std::string &s);

public:
    struct Face {
        std::vector<int> vertIndices;
        std::vector<int> texIndices;
        std::vector<int> normIndices;

        void addVertIdx(const std::string &indexStr);

        void addTexIdx(const std::string &indexStr);

        void addNormIdx(const std::string &indexStr);

        bool hasTexCoords() const;

        bool hasNormals() const;

        int size() const;

    private:
        static int getIndex(const std::string &index);

        friend std::ostream &operator<<(std::ostream &os, const Face &face);
    };

private:
    class MtlLoader {
    public:
        static std::unordered_map<std::string, ModelMaterial> load(const std::string &filename);

    };
};


