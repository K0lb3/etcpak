# etcpak

[![PyPI supported Python versions](https://img.shields.io/pypi/pyversions/etcpak.svg)](https://pypi.python.org/pypi/etcpak)
[![Win/Mac/Linux](https://img.shields.io/badge/platform-windows%20%7C%20macos%20%7C%20linux-informational)]()
[![MIT](https://img.shields.io/pypi/l/etcpak.svg)](https://github.com/K0lb3/etcpak/blob/master/LICENSE)
[![Build Status](https://github.com/K0lb3/etcpak/workflows/Test%20and%20Publish/badge.svg?branch=master)](https://github.com/K0lb3/etcpak/actions?query=workflow%3A%22Test+and+Publish%22)

A python wrapper for [etcpak](https://github.com/wolfpld/etcpak)

Some changes were made to the original code to make it cross-platform compatible.

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
compressed = etcpak.compress_to_dxt5(img_data, img.width, img.height)
```

## Functions

* all functions accept only arguments, no keywords
* the given data has to be RGBA for the RGB function as well

### compress_to_dxt1

*Compresses RGBA to DXT1*

:param data: RGBA data of the image
:type data: bytes
:param width: width of the image
:type width: int
:param height: height of the image
:type height: int
:returns: compressed data
:rtype: bytes"


### compress_to_dxt1_dither

*Compresses RGBA to DXT1 Dither*

:param data: RGBA data of the image
:type data: bytes
:param width: width of the image
:type width: int
:param height: height of the image
:type height: int
:returns: compressed data
:rtype: bytes"


### compress_to_dxt5

*Compresses RGBA to DXT5*

:param data: RGBA data of the image
:type data: bytes
:param width: width of the image
:type width: int
:param height: height of the image
:type height: int
:returns: compressed data
:rtype: bytes"


### compress_to_etc1

*Compresses RGBA to ETC1 RGB*

:param data: RGBA data of the image
:type data: bytes
:param width: width of the image
:type width: int
:param height: height of the image
:type height: int
:returns: compressed data
:rtype: bytes"


### compress_to_etc1_dither

*Compresses RGBA to ETC1 Dither*

:param data: RGBA data of the image
:type data: bytes
:param width: width of the image
:type width: int
:param height: height of the image
:type height: int
:returns: compressed data
:rtype: bytes"


### compress_to_etc1_alpha

*Compresses A to ETC1 Alpha*

:param data: RGBA data of the image
:type data: bytes
:param width: width of the image
:type width: int
:param height: height of the image
:type height: int
:returns: compressed data
:rtype: bytes"


### compress_to_etc2_rgb

*Compresses RGBA to ETC2 RGB*

:param data: RGBA data of the image
:type data: bytes
:param width: width of the image
:type width: int
:param height: height of the image
:type height: int
:returns: compressed data
:rtype: bytes"


### compress_to_etc2_rgba

*Compresses RGBA to ETC2 RGBA*

:param data: RGBA data of the image
:type data: bytes
:param width: width of the image
:type width: int
:param height: height of the image
:type height: int
:returns: compressed data
:rtype: bytes"


### compress_to_etc2_alpha

*Compresses RGBA to ETC2 Alpha*

:param data: RGBA data of the image
:type data: bytes
:param width: width of the image
:type width: int
:param height: height of the image
:type height: int
:returns: compressed data
:rtype: bytes"
