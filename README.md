# etcpak

[![PyPI supported Python versions](https://img.shields.io/pypi/pyversions/etcpak.svg)](https://pypi.python.org/pypi/etcpak)
[![Win/Mac/Linux](https://img.shields.io/badge/platform-windows%20%7C%20macos%20%7C%20linux-informational)]()
[![MIT](https://img.shields.io/pypi/l/etcpak.svg)](https://github.com/K0lb3/etcpak/blob/master/LICENSE)
[![Build Status](https://github.com/K0lb3/etcpak/workflows/CI/badge.svg?branch=master)](https://github.com/K0lb3/etcpak/actions?query=workflow%3A%22CI%22)

A python wrapper for [wolfpld/etcpak](https://github.com/wolfpld/etcpak)
All relevant function  and class documentation was taken from [wolfpld/etcpak](https://github.com/wolfpld/etcpak).

1. [Installation](https://github.com/K0lb3/etcpak#installation)
2. [Example](https://github.com/K0lb3/etcpak#example)
3. [Functions](https://github.com/K0lb3/etcpak#functions)

## Installation

```cmd
pip install etcpak
```

or download/clone the git and use

```cmd
python setup.py install
```

## Example

```python
from PIL import Image
import etcpak

# load image
img = Image.open(file_path)

# get image data
img_data = img.convert("RGBA").tobytes()

# compress data
compressed = etcpak.compress_bc3(img_data, img.width, img.height)
```

__composite image for format comparission__
```python
import os
import etcpak
import texture2ddecoder
from PIL import Image

FORMATS = [
    ("DXT1", etcpak.compress_bc1, texture2ddecoder.decode_bc1),
    ("DXT1 Dither", etcpak.compress_bc1_dither, texture2ddecoder.decode_bc1),
    ("DXT5", etcpak.compress_bc3, texture2ddecoder.decode_bc3),
    ("ETC1", etcpak.compress_etc1_rgb, texture2ddecoder.decode_etc1),
    ("ETC1 Dither", etcpak.compress_etc1_rgb_dither, texture2ddecoder.decode_etc1),
    ("ETC2 RGB", etcpak.compress_etc2_rgb, texture2ddecoder.decode_etc2),
    ("ETC2 RGBA", etcpak.compress_etc2_rgba, texture2ddecoder.decode_etc2a8)
]

p = "S:\\Pictures"
for fp in os.listdir(p):
    if not fp[-4:] in [".png", ".jpg", ".bmp", "jpeg"]:
        continue
    # load image and adjust format and size
    print(fp)
    img = Image.open(os.path.join(p, fp)).convert("RGBA")
    img = img.crop((0,0,img.width-img.width%4, img.height-img.height%4))
    
    # create composite image
    comp = Image.new("RGBA", (img.width*8, img.height))
    comp.paste(img, (0, 0))
    print(img.width * img.height * 4)

    # iterate over all formats
    for i, (name, enc, dec) in enumerate(FORMATS):
        print(name)
        # make sure that the channel order is correct for the compression
        if name[:3] == "DXT":
            raw = img.tobytes()
        elif name[:3] == "ETC":
            r,g,b,a = img.split()
            raw = Image.merge('RGBA', (b,g,r,a)).tobytes()
        
        # compress
        data = enc(raw, img.width, img.height)

        # decompress
        dimg = Image.frombytes("RGBA", img.size, dec(data, img.width, img.height), "raw", "BGRA")

        # add to composite image
        comp.paste(dimg, (img.width*(i+1), 0))

    # save composite image
    comp.save(os.path.splitext(fp)[0]+".png")
```

## Functions

* all functions accept only arguments, no keywords
* **the data has to be RGBA/BGRA for the RGB functions as well**
* **all __DXT__ compressions require data in the __RGBA__ format**
* **all __ETC__ compressions require data in the __BGRA__ format**

see [etcpak/__init__.pyi](./etcpak/__init__.pyi)