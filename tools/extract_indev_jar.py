#!/usr/bin/env python3
"""
Copies the textures out of an Indev minecraft.jar (e.g. in-20100223.jar)
into the "minecraft" resource pack, so they become the game's default look.

Indev kept several textures in different places than later versions, so
they are renamed to the paths the game expects. Indev's grass and leaves
are already green, so white grass/foliage color maps are written as well
to keep the game from tinting them a second time.

Usage: python3 tools/extract_indev_jar.py path/to/in-20100223.jar [destination]
"""

import io
import os
import sys
import zipfile

# path in the Indev jar -> path in the resource pack
MAPPING = {
    "terrain.png": "terrain.png",
    "particles.png": "particles.png",
    "default.png": "font/default.png",
    "clouds.png": "environment/clouds.png",
    "rain.png": "environment/rain.png",
    "shadow.png": "misc/shadow.png",
    "water.png": "misc/water.png",
    "char.png": "mob/char.png",
    "dirt.png": "gui/background.png",
    "terrain/sun.png": "terrain/sun.png",
    "terrain/moon.png": "terrain/moon.png",
}

# whole folders that keep the same layout
FOLDERS = ["armor/", "art/", "gui/", "item/", "mob/"]

# not used: the Indev logo (the game has its own), and textures the game doesn't use
SKIP = {"gui/logo.png"}


def write_file(destination, rel_path, data):
    out_path = os.path.join(destination, *rel_path.split("/"))
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    with open(out_path, "wb") as f:
        f.write(data)
    print(f"  {rel_path}")


def write_white_colormap(destination, rel_path):
    try:
        from PIL import Image
    except ImportError:
        print(f"Warning: Pillow is not installed, skipping {rel_path}. Grass and leaves may look too dark.")
        return
    buf = io.BytesIO()
    Image.new("RGB", (256, 256), (255, 255, 255)).save(buf, "PNG")
    write_file(destination, rel_path, buf.getvalue())


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 1

    jar_path = sys.argv[1]
    script_dir = os.path.dirname(os.path.abspath(__file__))
    destination = sys.argv[2] if len(sys.argv) > 2 else os.path.join(script_dir, "..", "game", "assets", "resource_packs", "minecraft")

    if not os.path.isfile(jar_path):
        print(f"Error: {jar_path} does not exist.")
        return 1

    print(f"Extracting Indev textures to {os.path.normpath(destination)}:")
    with zipfile.ZipFile(jar_path, "r") as jar:
        names = set(jar.namelist())
        for name in sorted(names):
            if name.endswith("/") or name in SKIP or ".." in name:
                continue
            if name in MAPPING:
                write_file(destination, MAPPING[name], jar.read(name))
            elif name.endswith(".png") and any(name.startswith(folder) for folder in FOLDERS):
                write_file(destination, name, jar.read(name))

    write_white_colormap(destination, "misc/grasscolor.png")
    write_white_colormap(destination, "misc/foliagecolor.png")

    print("Done.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
