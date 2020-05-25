#include "Color.hpp"


const Color Color::BLACK{0.f, 0.f, 0.f};
const Color Color::RED{1.f, 0.f, 0.f};
const Color Color::YELLOW{1.f, 1.f, 0.f};
const Color Color::GREEN{0.f, 1.f, 0.f};
const Color Color::CYAN{0.f, 1.f, 1.f};
const Color Color::BLUE{0.f, 0.f, 1.f};
const Color Color::MAGENTA{1.f, 0.f, 1.f};
const Color Color::WHITE{1.f, 1.f, 1.f};

Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}

Color::Color() : r(0), g(0), b(0) {}

Color::Color(uint8_t r, uint8_t g, uint8_t b)
        : Color(static_cast<float>(r) / 255,
                static_cast<float>(g) / 255,
                static_cast<float>(b) / 255) {}


void Color::toArray(float rgba[4]) const {
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = 1.f;
}

std::ostream &operator<<(std::ostream &os, const Color &color) {
    os << "Color(r: " << color.r << " g: " << color.g << " b: " << color.b << ")";
    return os;
}
