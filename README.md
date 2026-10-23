# SomaticVR Orion firmware - SlimeVR 0.7.4 port

Orion hardware support ported onto SlimeVR Tracker ESP 0.7.4. The working firmware identifies as `0.7.4-Orion-port-udpfix`.

## Build

Use Python 3.12, Git and PlatformIO:

```sh
python -m pip install -r requirements-build.txt
python -m platformio run -e BOARD_SOMATICVR_ORION
```

Outputs are in `.pio/build/BOARD_SOMATICVR_ORION/`:

- `firmware.hex`: complete, verified Intel HEX for Somatic Eros Flasher.
- `firmware.hex.sha256`: checksum of the HEX.
- `firmware.bin`: application only; do not flash this at address zero.

The build rejects application binaries larger than their partition and verifies the packaged HEX against its input bytes. Link-time optimization is required to fit the existing partition layout. You can override the displayed version with the `FIRMWARE_VERSION` environment variable.

## Hardware and changes

Preserves Orion ESP32-C3 pins, quaternion sensor mounting, power latch, button, vibration, charging/USB-PD, battery monitoring and LEDs. Uses upstream protocol 23 and acceleration correction, calibration-file validation and logging. The upstream early recovery hook is intentionally disabled so Orion retains its original startup sequence. Orion uses `BOARD_CUSTOM` (4), avoiding ID 27's collision with upstream ESP32-C6 Supermini.

Restores the Orion ESP32 oversized UDP packet workaround using `NetworkUDP::clear()`. Unread packet data otherwise prevents subsequent packets from being received under Arduino ESP32 3.0.1.

## Validation

The user reported the udpfix firmware working on their trackers after the earlier boot and connection issues were corrected. This is user testing, not a complete hardware qualification. Check power, charging, battery, rotation, sustained Wi-Fi data and mounting calibration on one tracker before updating a set.

The original `port-test`, `bootfix1`, and `sizefix` images are superseded. No device dumps, Wi-Fi credentials, local build environments or diagnostic logs are included in this source tree.

## Provenance and licensing

Based on SlimeVR/SlimeVR-Tracker-ESP 0.7.4 and SomaticVR/SomaticVR-Tracker-ESP v0.7.2-Orion-2 supplied by the user. Beta-3 source was unavailable. Preserve the upstream and SomaticVR copyright notices. See LICENSE-MIT and LICENSE-APACHE.

The vendor update URL is retained from Orion. Updating from that vendor feed may replace this custom port.
