import sys
from setuptools import setup, Extension
from typing import List

ETCPAK_SOURCES = [
    "bc7enc.cpp",
    # "bcdec.c", mac issue, fixed by moving define to compiler define arg
    "Bitmap.cpp",
    # "BitmapDownsampled.cpp",
    "BlockData.cpp",
    "ColorSpace.cpp",
    # "DataProvider.cpp",
    "Debug.cpp",
    "Dither.cpp",
    "Error.cpp",
    "mmap.cpp",
    "ProcessDxtc.cpp",
    "ProcessRGB.cpp",
    "System.cpp",
    "Tables.cpp",
    "TaskDispatch.cpp",
    "Timing.cpp",
]

with open("README.md", "r") as fh:
    long_description = fh.read()

SYSTEM_COMPILE_ARGS: List[str]
if sys.platform == "win32":
    SYSTEM_COMPILE_ARGS = ["/srd:c++20", "/Zc:strictStrings-"]
else:
    SYSTEM_COMPILE_ARGS = ["-std=c++20"]

setup(
    name="etcpak",
    description="python wrapper for etcpak",
    author="K0lb3",
    version="0.9.9",
    packages=["etcpak"],
    package_data={"etcpak": ["__init__.py", "__init__.pyi"]},
    keywords=["etc", "dxt", "texture", "python-c"],
    classifiers=[
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
        "Intended Audience :: Developers",
        "Programming Language :: Python",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Topic :: Multimedia :: Graphics",
    ],
    url="https://github.com/K0lb3/etcpak",
    download_url="https://github.com/K0lb3/etcpak/tarball/master",
    long_description=long_description,
    long_description_content_type="text/markdown",
    ext_modules=[
        Extension(
            "etcpak._etcpak",
            [
                "src/pylink.cpp",
                "src/dummy.cpp",
                *[f"src/etcpak/{src}" for src in ETCPAK_SOURCES],
            ],
            language="c++",
            include_dirs=[
                "src/etcpak",
            ],
            extra_compile_args=[
                *SYSTEM_COMPILE_ARGS,
                "-DNOMINMAX",
                "-DNDEBUG",
                "-DNO_GZIP",
                # Mac fix due to .c problem
                "-DBCDEC_IMPLEMENTATION=1",
            ],
        )
    ],
)
