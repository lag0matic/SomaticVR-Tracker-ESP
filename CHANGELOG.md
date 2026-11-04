# Change history and provenance

## 0.7.4-Orion-battfix2 (candidate)

- Fix signed ramp division/comparison: descending LED fades no longer become a large positive step. Clamp each brightness update to its actual target, handle sub-tick durations, and guarantee progress for small ramps.
- Starting charging explicitly initializes its breathing ramp, even if the prior state was a downward fade. Charging priority still yields to shutdown and full status.
- Remove the inverted charger interrupt gate. Poll every 250 ms after the existing startup delay even when INT is low. No new busy polling is added.
- Correct PD debug voltage/current scaling for fixed PDO and PPS units. Remove the raw-voltage filter that excluded typical ready contracts. These changes do not alter negotiated voltages or current settings.
- Watchdog disable and board ID collision avoidance already existed in battfix1 and remain unchanged. The source driver targets BQ24195; a report referring to BQ24196 is not physical chip identification.
- Host tests cover descending and ascending ramps, endpoint clamping, short durations, PD unit conversions, power-latch reset policy and PD failure recovery. New combined hardware behavior remains unverified.


## 0.7.4-Orion-battfix1 (candidate)

Base: the user-tested 0.7.4 udpfix port, commit `7682e05` on `codex/orion-0.7.4`.

SomaticVR reference: `chargerFix` commit `0c0c8b67da36cb549025719ba27d28c6bf4aed17`, compared with Orion-2 `a6412e6`.

### SlimeVR 0.7.4 port

The supplied SlimeVR 0.7.4 source replaces the Orion-2 core while retaining Orion hardware drivers and board settings. Includes protocol 23; consistent acceleration/sensor mounting transforms; invalid/short calibration file rejection; magnetometer timestep support; upstream sensor temperature scheduling; buffered serial logging; disabled-by-default profiling; configuration factory reset and serial flashing commands. General quaternion mounting remains necessary for Orion's 180-degree Y sensor rotation.

Orion ESP32-C3 pins, charger and USB-PD drivers, charging/handoff status, battery scaling, LEDs, vibration and Wi-Fi hostname/transmit-power behavior were carried over. The upstream early recovery-console hook is disabled; its reset-request flag remains false. The custom board identity is 4 instead of the conflicting 27.

### SomaticVR battery and button updates

- Input current limit changes from 2 A to 1.5 A. This is not the battery charge-current setting.
- Setup explicitly disables the charger watchdog so it does not later revert charger settings when the processor stops servicing it. This is SomaticVR's stated purpose, not a claim that every historical charging failure was caused by that watchdog.
- Shutdown uses elapsed time (2 s arm/release, 10 s full off) rather than polling counts.
- Adopts GPIO hold for the latch, with the corrections below.

### Additional fixes

- `PowerLatch.h` centralizes all firmware latch writes. Output level and direction are prepared while held, then hold is released and restored. This avoids exposing a default/reset output during release.
- Both charge-driven latch HIGH and deliberate shutdown LOW use that helper; plain writes to a held pad are no longer used.
- Cold power-on, brownout, unknown reset and deep-sleep entry require a pressed button to latch immediately. Other reset reasons retain power, so software/OTA/watchdog/USB/JTAG reboots do not select LOW just because the button is up. USB-only cold starts retain the vendor charging-only startup policy; normal charger polling can subsequently assert the latch.
- Fixes the upstream full-off branch's redundant conditional so SHUTDOWN status is cleared unconditionally before full off.
- PD pin sampling and alert servicing return on instability/read errors instead of spinning indefinitely. Shared I2C transactions can still take their existing bounded timeout; charging is not a separate real-time task.
- Retains udpfix: oversized UDP datagrams are discarded completely in both discovery and connected paths.
- Retains `-Os` plus compile/link LTO, final image partition checks and verified HEX packaging.

### Deliberately excluded experiments

`charge-test1`, `charge-test2`, and `charge-test3` were diagnostic builds. Their termination/startup-status/interrupt-polling changes, read-only charger commands, temperature-getter rewrite and RAM-only logging policy are not included here. Test3 was reported to power off unexpectedly, without a confirmed root cause. This candidate keeps SomaticVR's termination and temperature-getter behavior and the stable udpfix buffered logger. Only the bounded PD retry safeguard is retained from those experiments.

The original charging/handoff failure was intermittent; older udpfix firmware also subsequently charged successfully. No claim is made that this candidate resolves every overnight charging fault or the third-party report of post-flash lockups.

### Software and hardware evidence

In the separate battfix1 comparison, vendor charger setup/update, button update, PD setup/update/run, main setup/loop and loopTask compiled routines matched the supplied `Orion-Firmware-0.7.2-battfix1.bin` exactly. The complete rebuilt application was not byte-identical: build metadata, data locations and other framework code differed. The supplied image identifies as `0.7.1-battfix1` despite its filename.

This branch requires a new one-tracker hardware test. Prior successful udpfix use and mock tests do not qualify the newly combined power behavior.
