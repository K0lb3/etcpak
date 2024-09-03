import os
import etcpak
import texture2ddecoder
from PIL import Image
import imagehash


def _test_helper(
    name: str, channels: int, encode_func: callable, decode_func: callable
):
    p = os.path.dirname(os.path.realpath(__file__))
    for fp in os.listdir(p):
        if not fp[-4:] in [".png", ".jpg", ".bmp", "jpeg"]:
            continue
        img = Image.open(os.path.join(p, fp)).convert("RGBA")
        img = img.crop((0, 0, img.width - img.width % 4, img.height - img.height % 4))

        raw = img.tobytes()
        # compress
        data = encode_func(raw, img.width, img.height)
        # decompress
        dimg = Image.frombytes(
            "RGBA", img.size, decode_func(data, img.width, img.height), "raw", "BGRA"
        )

        # compare to original
        if channels in [1, 2]:
            # todo
            continue
        elif channels == 3:
            img = img.convert("RGB")
            dimg = dimg.convert("RGB")

        assert compare_images(img, dimg), f"Failed {name}: {fp}"


def compare_images(im1: Image.Image, im2: Image.Image) -> bool:
    # lossy compression, so some leeway is allowed
    return abs(imagehash.average_hash(im1) - imagehash.average_hash(im2)) <= 1


def test_bc1():
    _test_helper("BC1", 3, etcpak.compress_bc1, texture2ddecoder.decode_bc1)


def test_bc1_dither():
    _test_helper(
        "BC1 Dither", 3, etcpak.compress_bc1_dither, texture2ddecoder.decode_bc1
    )


def test_bc3():
    _test_helper("BC3", 4, etcpak.compress_bc3, texture2ddecoder.decode_bc3)


def test_bc4():
    _test_helper("BC4", 1, etcpak.compress_bc4, texture2ddecoder.decode_bc4)


def test_bc5():
    _test_helper("BC5", 3, etcpak.compress_bc5, texture2ddecoder.decode_bc5)


def test_bc7():
    _test_helper("BC7", 4, etcpak.compress_bc7, texture2ddecoder.decode_bc7)


def test_etc1_rgb():
    _test_helper("ETC1 RGB", 3, etcpak.compress_etc1_rgb, texture2ddecoder.decode_etc1)


def test_etc1_rgb_dither():
    _test_helper(
        "ETC1 RGB Dither",
        3,
        etcpak.compress_etc1_rgb_dither,
        texture2ddecoder.decode_etc1,
    )


def test_etc2_rgb():
    _test_helper("ETC2 RGB", 3, etcpak.compress_etc2_rgb, texture2ddecoder.decode_etc2)


def test_etc2_rgba():
    _test_helper(
        "ETC2 RGBA", 4, etcpak.compress_etc2_rgba, texture2ddecoder.decode_etc2a8
    )


if __name__ == "__main__":
    for name in dir():
        if name.startswith("test_"):
            globals()[name]()
