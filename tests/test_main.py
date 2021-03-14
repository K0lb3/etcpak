import os
import etcpak
import texture2ddecoder
from PIL import Image, ImageChops
import imagehash

FORMATS = [
    ("DXT1", etcpak.compress_to_dxt1, texture2ddecoder.decode_bc1),
    ("DXT1 Dither", etcpak.compress_to_dxt1_dither, texture2ddecoder.decode_bc1),
    ("DXT5", etcpak.compress_to_dxt5, texture2ddecoder.decode_bc3),
    ("ETC1", etcpak.compress_to_etc1, texture2ddecoder.decode_etc1),
    ("ETC1 Dither", etcpak.compress_to_etc1_dither, texture2ddecoder.decode_etc1),
    ("ETC2 RGB", etcpak.compress_to_etc2_rgb, texture2ddecoder.decode_etc2),
    ("ETC2 RGBA", etcpak.compress_to_etc2_rgba, texture2ddecoder.decode_etc2a8)
]

def test_main():
    p = os.path.dirname(os.path.realpath(__file__))
    for fp in os.listdir(p):
        if not fp[-4:] in [".png", ".jpg", ".bmp", "jpeg"]:
            continue
        # load image and adjust format and size
        img = Image.open(os.path.join(p, fp)).convert("RGBA")
        img = img.crop((0,0,img.width-img.width%4, img.height-img.height%4))
        # iterate over all formats
        for i, (name, enc, dec) in enumerate(FORMATS):
            if name[:3] == "DXT":
                raw = img.tobytes()
            elif name[:3] == "ETC":
                r,g,b,a = img.split()
                raw = Image.merge('RGBA', (b,g,r,a)).tobytes()
            # compress
            data = enc(raw, img.width, img.height)
            # decompress
            dimg = Image.frombytes("RGBA", img.size, dec(data, img.width, img.height), "raw", "BGRA")
            # compare to original
            if name in ["DXT5","ETC2 RGBA"]:
                assert(compare_images(img,dimg))
            else:
                assert(compare_images(img.convert("RGB"), dimg.convert("RGB")))

def compare_images(im1,im2):
    # lossy compression, so some leeway is allowed
    return abs(imagehash.average_hash(im1) - imagehash.average_hash(im2)) <= 1

if __name__ == "__main__":
    test_main()