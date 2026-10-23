Import("env")
import struct
from pathlib import Path

def check_image(source, target, env):
    image = Path(str(target[0]))
    table = Path(env.subst("$BUILD_DIR")) / "partitions.bin"
    partitions = table.read_bytes()
    app_offset = int(str(env.get("ESP32_APP_OFFSET", "0x10000")), 0)
    for pos in range(0, len(partitions), 32):
        entry = partitions[pos:pos+32]
        if len(entry) != 32 or entry[:2] != b"\xaaP":
            break
        _, kind, subtype, offset, size, label, flags = struct.unpack("<HBBII16sI", entry)
        if kind == 0 and offset == app_offset:
            actual = image.stat().st_size
            if actual > size:
                raise RuntimeError(f"Application binary {actual} bytes exceeds partition {size} bytes")
            print(f"Verified final binary: {actual}/{size} bytes; headroom {size-actual}")
            return
    raise RuntimeError("Cannot locate application partition for final size validation")

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", check_image)
