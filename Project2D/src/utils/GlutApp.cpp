#include "GlutApp.hpp"

Configuration GlutApp::config{};
AppListener *GlutApp::listener = nullptr;
SystemClock::time_point GlutApp::tp1 = SystemClock::now();
SystemClock::time_point GlutApp::tp2 = SystemClock::now();
