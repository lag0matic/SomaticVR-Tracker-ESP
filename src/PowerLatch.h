#pragma once
#include <Arduino.h>
#include "driver/gpio.h"
#include "esp_system.h"

namespace SlimeVR {
inline bool shouldLatchAtStartup(bool buttonPressed, esp_reset_reason_t reason) {
    // USB-only cold starts retain Orion's charging-only behavior. Reboots of a
    // running application must not drop the latch merely because the button is up.
    return buttonPressed || (reason != ESP_RST_POWERON && reason != ESP_RST_BROWNOUT
        && reason != ESP_RST_DEEPSLEEP && reason != ESP_RST_UNKNOWN);
}
inline void setPowerLatch(bool enabled) {
    const auto pin = static_cast<gpio_num_t>(PIN_ENABLE_LATCH);
    // Configure the underlying output while the pad is held. Releasing hold then
    // exposes the desired level, rather than a reset/default low output.
    gpio_set_level(pin, enabled ? 1 : 0);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    gpio_hold_dis(pin);
    gpio_hold_en(pin);
}
} // namespace SlimeVR
