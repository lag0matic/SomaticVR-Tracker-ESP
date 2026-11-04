#pragma once
#include <cstdint>
namespace SlimeVR {
constexpr float pdVoltage(uint16_t value, bool pps) { return value / (pps ? 50.0f : 20.0f); }
constexpr float pdCurrent(uint16_t value, bool pps) { return value / (pps ? 20.0f : 100.0f); }
}
