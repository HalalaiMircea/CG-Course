#pragma once

#include <ostream>
#include <cstdint>

struct Color {
    static const Color WHITE, BLACK, RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, GREY;

    float r, g, b, a;

    Color();

    Color(float r, float g, float b, float a = 1.0f);

    Color(uint8_t r, uint8_t g, uint8_t b, float a = 1.0f);

    void toArray(float rgba[4]) const;

    friend std::ostream &operator<<(std::ostream &os, const Color &color);
};
