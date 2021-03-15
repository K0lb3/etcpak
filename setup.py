from setuptools import setup, Extension, find_packages
import os

with open("README.md", "r") as fh:
    long_description = fh.read()

setup(
    name="etcpak",
    description="python wrapper for etcpak",
    author="K0lb3",
    version="0.9.2",
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
        "Topic :: Multimedia :: Graphics",
    ],
    url="https://github.com/K0lb3/etcpak",
    download_url="https://github.com/K0lb3/etcpak/tarball/master",
    long_description=long_description,
    long_description_content_type="text/markdown",
    ext_modules=[
        Extension(
            "etcpak",
            [
                "src/pylink.cpp",
                "src/etcpak/ProcessDxtc.cpp",
                "src/etcpak/ProcessRGB.cpp",
                "src/etcpak/Tables.cpp",
                "src/etcpak/Dither.cpp"
            ],
            language="c++",
            include_dirs=["src/etcpak","src/etcpak/libpng","src/etcpak/zlib","src/etcpak/lz4"],
            extra_compile_args=["-std=c++11"],
        )
    ],
)
