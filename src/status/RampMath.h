#pragma once
#include <algorithm>
#include <cstdint>
namespace SlimeVR {
inline int rampStep(unsigned int start, unsigned int end, unsigned long duration, unsigned long tick) {
    const int64_t ticks = std::max<int64_t>(1, duration / tick);
    const int64_t delta = static_cast<int64_t>(end) - start;
    int step = static_cast<int>(delta / ticks);
    if (step == 0 && delta != 0) step = delta > 0 ? 1 : -1;
    return step;
}
inline unsigned int advanceRamp(unsigned int current, int step, unsigned int target) {
    const int64_t next = static_cast<int64_t>(current) + step;
    return static_cast<unsigned int>(step < 0 ? std::max<int64_t>(next, target)
        : std::min<int64_t>(next, target));
}
}
