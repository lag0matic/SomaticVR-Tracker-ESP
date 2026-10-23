Import("env")
import hashlib
import struct
from pathlib import Path

def package_hex(source, target, env):
    build = Path(env.subst("$BUILD_DIR"))
    application = Path(str(target[0])).read_bytes()
    table = (build / "partitions.bin").read_bytes()
    app_offset = int(str(env.get("ESP32_APP_OFFSET", "0x10000")), 0)
    entries = [struct.unpack("<HBBII16sI", table[i:i+32]) for i in range(0, len(table), 32) if table[i:i+2] == b"\xaaP"]
    partition = next(e for e in entries if e[1] == 0 and e[3] == app_offset)
    if len(application) > partition[4]:
        raise RuntimeError("Application binary exceeds flash partition")
    framework = Path(env.PioPlatform().get_package_dir("framework-arduinoespressif32"))
    parts = [(0, build / "bootloader.bin"), (0x8000, build / "partitions.bin"), (0xe000, framework / "tools/partitions/boot_app0.bin"), (app_offset, Path(str(target[0])))]
    image = bytearray(b"\xff" * (app_offset + len(application)))
    for offset, path in parts:
        payload = path.read_bytes()
        image[offset:offset+len(payload)] = payload
    def record(address, kind, payload):
        raw = bytes([len(payload)]) + address.to_bytes(2, "big") + bytes([kind]) + payload
        return ":" + (raw + bytes([-sum(raw) & 255])).hex().upper()
    lines = []
    for address in range(0, len(image), 16):
        if address % 65536 == 0:
            lines.append(record(0, 4, (address >> 16).to_bytes(2, "big")))
        lines.append(record(address & 65535, 0, image[address:address+16]))
    lines.append(record(0, 1, b""))
    output = build / "firmware.hex"
    output.write_text("\n".join(lines) + "\n", encoding="ascii")
    decoded = bytearray()
    base = 0
    for line in output.read_text().splitlines():
        raw = bytes.fromhex(line[1:])
        if sum(raw) % 256 or len(raw) != raw[0]+5:
            raise RuntimeError("Invalid generated Intel HEX record")
        if raw[3] == 4:
            base = int.from_bytes(raw[4:6], "big") << 16
        elif raw[3] == 0:
            if len(decoded) != base + int.from_bytes(raw[1:3], "big"):
                raise RuntimeError("Unexpected HEX address")
            decoded.extend(raw[4:4+raw[0]])
    if decoded != image:
        raise RuntimeError("HEX round-trip verification failed")
    output.with_suffix(".hex.sha256").write_text(hashlib.sha256(output.read_bytes()).hexdigest() + "  firmware.hex\n")
    print("Packaged and verified " + str(output))

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", package_hex)
