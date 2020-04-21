#pragma once

struct Color {
    float r, g, b;

    Color() : r(0), g(0), b(0) {}

    Color(float r, float g, float b) : r(r), g(g), b(b) {}

    Color(uint8_t r, uint8_t g, uint8_t b)
            : Color(static_cast<float>(r) / 255,
                    static_cast<float>(g) / 255,
                    static_cast<float>(b) / 255) {}

    static const Color WHITE, BLACK, RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA;
};

const Color Color::BLACK{0.f, 0.f, 0.f};
const Color Color::RED{1.f, 0.f, 0.f};
const Color Color::YELLOW{1.f, 1.f, 0.f};
const Color Color::GREEN{0.f, 1.f, 0.f};
const Color Color::CYAN{0.f, 1.f, 1.f};
const Color Color::BLUE{0.f, 0.f, 1.f};
const Color Color::MAGENTA{1.f, 0.f, 1.f};
const Color Color::WHITE{1.f, 1.f, 1.f};