#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <GL/gl.h>

class ObjLoader {
public:
    static GLuint loadModel(const std::string &filename);

private:
    static std::vector<std::string> splitString(const std::string &str, char delimiter);


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

        static int getIndex(const std::string &index);

        friend std::ostream &operator<<(std::ostream &os, const Face &face);
    };
};
