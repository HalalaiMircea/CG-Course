#include "GlutApp3D.hpp"

Configuration GlutApp3D::config{};
AppListener *GlutApp3D::listener = nullptr;
SystemClock::time_point GlutApp3D::tp1 = SystemClock::now();
SystemClock::time_point GlutApp3D::tp2 = SystemClock::now();
float GlutApp3D::deltaTime = 0;

float GlutApp3D::getDeltaTime() {
    return deltaTime;
}
