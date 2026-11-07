# SomaticVR Orion - Experimental SlimeVR 0.7.4 Firmware

> **Experimental beta branch. This is a community firmware build for SomaticVR Orion trackers, not an official release from SomaticVR or SlimeVR. Bugs may remain undiscovered.**

Current firmware version: **`0.7.4-Orion-battfix2`**  
Development branch: **`codex/orion-0.7.4-battfix`**

## About this branch

This project brings SomaticVR Orion's custom hardware support and firmware behavior into a SlimeVR Tracker ESP 0.7.4-based build. It integrates SomaticVR's Orion-specific changes, including the battery, charging, button, and power-handling updates published on their `chargerFix` branch, along with additional fixes developed while reviewing and testing that code.

The aim is to retain Orion's hardware functionality while supporting the newer SlimeVR firmware behavior. This remains an integration effort under active testing; it should not be treated as a finished or fully validated release.

For implementation details and source provenance, see [CHANGELOG.md](CHANGELOG.md).

## Experimental status and testing

The firmware has passed local builds, image-size and HEX validation, and software regression tests. The maintainer has also reported several multi-hour sessions on a test tracker without significant issues, including connection, motion reporting, battery operation, and charging tests.

That is limited testing on a small set of hardware, not a guarantee of reliability across all trackers, batteries, chargers, or charging-stack configurations. Other bugs may yet be found, including regressions that only appear after extended use or under different operating conditions. Charging, stack handoff, restart behavior, and long-term stability still warrant continued testing.

Keep a known-working firmware backup. Try this build on one tracker before updating a full set, confirm the reported firmware version, and check normal operation and charging afterward. If unexpected behavior occurs, record the firmware version, tracker configuration, and steps needed to reproduce it.

## AI-assisted development

**This branch was developed with assistance from OpenAI Codex.** AI was used to help compare source code, investigate issues, implement changes, write tests, and prepare documentation.

AI-assisted code and analysis can contain mistakes. Successful compilation, automated tests, and limited hardware testing do not prove that every change is correct or that the firmware is safe or reliable in every situation. Independent source review and additional hardware testing are encouraged. Known and undiscovered defects may remain.

This firmware is provided as-is, without warranty, subject to the included licenses. It is not endorsed or certified by SomaticVR or SlimeVR.

## Building

Requires Git, Python 3.12, and PlatformIO. Check out this branch, then run:

```sh
python -m pip install -r requirements-build.txt
python -m platformio run -e BOARD_SOMATICVR_ORION
```

The default firmware version is `0.7.4-Orion-battfix2`. The `FIRMWARE_VERSION` environment variable can override it; the Git revision is embedded separately. Downloading build dependencies requires internet access. Different build environments may produce different binary bytes.

Build outputs are written to `.pio/build/BOARD_SOMATICVR_ORION/`:

- **`firmware.hex`** - complete Intel HEX image for Somatic Eros Flasher.
- **`firmware.hex.sha256`** - checksum of the generated HEX image.
- **`firmware.bin`** - application-only image; **do not flash this file at address zero**.

The build checks that the application fits its partition and verifies the generated HEX against its input bytes. The merged HEX includes boot components and fills gaps with erased bytes; flashing may reset stored Wi-Fi settings depending on the flasher's behavior.

Software regression-test instructions are available in [test/battfix-host/README.md](test/battfix-host/README.md). These tests complement hardware testing; they do not replace it.

## Updates

The firmware retains SomaticVR's vendor update metadata. Updating through the vendor feed may replace this custom build with vendor firmware. This repository's workflow builds artifacts; it does not automatically publish releases.

## Credits and licenses

This work builds on:

- [SlimeVR Tracker ESP](https://github.com/SlimeVR/SlimeVR-Tracker-ESP), using the supplied 0.7.4 source.
- [SomaticVR Tracker ESP](https://github.com/SomaticVR/SomaticVR-Tracker-ESP), including Orion-2 hardware support and the published [`chargerFix` updates](https://github.com/SomaticVR/SomaticVR-Tracker-ESP/tree/chargerFix).

Original copyright notices and licenses are retained. See [LICENSE-MIT](LICENSE-MIT), [LICENSE-APACHE](LICENSE-APACHE), and the licenses accompanying individual vendored libraries.
