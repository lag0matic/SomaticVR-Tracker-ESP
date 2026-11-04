# SomaticVR Orion - SlimeVR 0.7.4 + battfix

Firmware version: **`0.7.4-Orion-battfix2`**. This branch combines our working 0.7.4 udpfix port with SomaticVR's `chargerFix` battery/button updates and additional power-latch and PD recovery fixes.

**Status: test firmware.** It builds and passes software regression tests. This combined version has not yet been qualified on physical trackers, across flash/OTA resets, or through overnight stack charging. Keep your known-good `0.7.4-Orion-port-udpfix` backup and test one tracker first.

## Additional battfix2 fixes

Fixes signed LED ramp arithmetic and charging-animation entry, polls the charger even while its interrupt is asserted, and corrects fixed-PDO/PPS diagnostic unit conversions. PD logs describe the negotiated current limit, not measured draw. See CHANGELOG.md.

## What changed

| Area | Changes from SomaticVR Orion-2 |
|---|---|
| SlimeVR 0.7.4 | Upstream protocol 23, sensor acceleration coordinate correction, calibration-file validation, magnetometer timestep handling, sensor timing behavior, buffered logging, benchmarks (disabled by default), updated configuration/reset/serial commands. Orion's quaternion sensor orientation and hardware integration are preserved. |
| ESP32 networking | Restores Orion's oversized UDP protection using `NetworkUDP::clear()`. Unread oversized datagrams otherwise prevent subsequent server packets from being received. |
| Battery updates from SomaticVR | Input current limit reduced from 2 A to 1.5 A; charger watchdog explicitly disabled during setup. Battery charge-current setting remains 1 A, charge voltage setting 4.2 V, and thermal-regulation setting 60 C. Other hardware protections and charge safety timer are retained. |
| Button handling from SomaticVR | Uses elapsed milliseconds rather than loop counts: normal shutdown arms at 2 seconds and occurs on release; full off at 10 seconds. |
| Power-latch fixes | All latch writes use a common helper that preloads the output before releasing GPIO hold and then restores hold. Charger-triggered HIGH transitions can now change a previously held LOW output. Startup distinguishes cold/charging-only entry from warm resets rather than requiring a held button after software reset. |
| USB-PD recovery | Unstable CC readings and I2C errors return to the main loop instead of retrying indefinitely. PD servicing makes one alert attempt per call and retries later. Individual I2C operations retain their existing timeout. |
| Build safeguards | Link-time optimization keeps the final binary inside the existing application partition. Build checks the actual binary size, produces a complete Intel HEX and verifies its checksums and decoded bytes. |
| Board identity | Uses upstream `BOARD_CUSTOM` (4), avoiding Orion's old ID 27 collision with ESP32-C6 Supermini. Orion pins and sensor mounting remain unchanged. |

See [CHANGELOG.md](CHANGELOG.md) for source provenance, startup behavior and differences from the earlier experimental builds.

## Build

Requires Git, Python 3.12 and PlatformIO. From this branch:

```sh
python -m pip install -r requirements-build.txt
python -m platformio run -e BOARD_SOMATICVR_ORION
```

The default build version is `0.7.4-Orion-battfix2`; `FIRMWARE_VERSION` can override it. Git revision is embedded separately. Dependency downloads require network access; byte-for-byte identity across different environments is not guaranteed.

Outputs in `.pio/build/BOARD_SOMATICVR_ORION/`:

- **`firmware.hex`**: merged Intel HEX for Somatic Eros Flasher.
- `firmware.hex.sha256`: checksum.
- `firmware.bin`: application-only image, **not** a file to flash at address zero.

The existing 4 MiB layout is retained: bootloader `0x0000`, partitions `0x8000`, OTA initialization `0xe000`, application `0x10000`. The merged HEX fills gaps with erased bytes, so the flasher may reset saved Wi-Fi settings. Keep a backup before flashing.

## Validation and first tracker test

Software tests exercise startup reset policy, latch operation order, high/low transitions, failed/unstable PD reads and recovery. See [host-test instructions](test/battfix-host/README.md). CI builds the firmware and runs these tests; it does not validate electrical behavior.

On one tracker, verify button-on/off on battery, cold USB connection, reboot after USB flashing, OTA reboot, sensor motion and sustained Wi-Fi transmission. Then test direct charging, two-tracker handoff, and overnight maintenance. Confirm the displayed firmware version before updating a set.

## AI-assisted development

This firmware was developed with assistance from OpenAI Codex. AI-assisted changes can contain errors; successful compilation and software tests do not establish hardware safety or reliability. Review the source and validate it on your hardware before wider use. This is a community fork, not an official SomaticVR or SlimeVR release. Provided as-is under the included licenses, without warranty.

## Credits and licensing

Based on SlimeVR Tracker ESP 0.7.4, SomaticVR Orion-2, and SomaticVR's `chargerFix` branch. Original copyright notices are retained. See LICENSE-MIT and LICENSE-APACHE, plus individual vendored-library licenses.

Vendor OTA/update metadata remains pointed at SomaticVR; using that feed may replace this custom build. This repository's workflow only builds artifacts and does not publish releases automatically. Device flash dumps, personal credentials and diagnostic logs are not included.
