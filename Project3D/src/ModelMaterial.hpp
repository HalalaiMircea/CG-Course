#pragma once

#include <string>
#include <ostream>
#include "utils/Color.hpp"


struct ModelMaterial {
    std::string id;

    Color ambient;
    Color diffuse;
    Color specular;
    Color emissive;

    float shininess = 0;

    friend std::ostream &operator<<(std::ostream &os, const ModelMaterial &material) {
        os << "id: " << material.id << "\nambient: " << material.ambient << "\ndiffuse: " << material.diffuse
           << "\nspecular: " << material.specular << "\nemissive: " << material.emissive << "\nshininess: "
           << material.shininess;
        return os;
    }
};
