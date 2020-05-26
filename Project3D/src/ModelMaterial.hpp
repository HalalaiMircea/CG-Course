#pragma once

#include <string>
#include <ostream>
#include "utils/Color.hpp"


struct ModelMaterial {
    std::string id = "default";

    Color ambient = Color::WHITE;
    Color diffuse = Color::WHITE;
    Color specular = Color::WHITE;
    Color emissive = Color::BLACK;

    float shininess = 0.f;

    friend std::ostream &operator<<(std::ostream &os, const ModelMaterial &material) {
        os << "id: " << material.id << "\nambient: " << material.ambient << "\ndiffuse: " << material.diffuse
           << "\nspecular: " << material.specular << "\nemissive: " << material.emissive << "\nshininess: "
           << material.shininess;
        return os;
    }
};
