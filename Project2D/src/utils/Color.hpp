#pragma once


#include <cstdint>

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
