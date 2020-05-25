#pragma once


#include <cstdint>

struct Color {
    static const Color WHITE, BLACK, RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA;

    float r, g, b;

    Color();

    Color(float r, float g, float b);

    Color(uint8_t r, uint8_t g, uint8_t b);
};
