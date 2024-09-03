from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

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


class CustomBuildExt(build_ext):
    def build_extensions(self):
        compiler_type = self.compiler.compiler_type
        if compiler_type == "msvc":
            # MSVC-specific compiler and linker flags
            for ext in self.extensions:
                ext.extra_compile_args.extend(
                    [
                        "/std:c++20",
                        "/Zc:strictStrings-",
                        "/DNOMINMAX",
                        "/GL",
                    ]
                )
                if "-amd64" in self.plat_name:
                    ext.extra_compile_args.extend(
                        ["/D__SSE4_1__", "/D__AVX2__", "/arch:AVX2"]
                    )
                ext.extra_link_args = ["/LTCG:incremental"]
        else:
            # For other compilers (e.g., GCC or Clang)
            if "-arm" in self.plat_name or "-aarch64" in self.plat_name:
                if "macosx" in self.plat_name:
                    native_arg = "-mcpu=apple-m1"
                else:
                    native_arg = "-mcpu=native"
            else:
                native_arg = "-march=native"

            for ext in self.extensions:
                ext.extra_compile_args.extend(["-std=c++20", native_arg])

        super().build_extensions()


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
                "-DNDEBUG",
                "-DNO_GZIP",
                # Mac fix due to .c problem
                "-DBCDEC_IMPLEMENTATION=1",
            ],
        )
    ],
    cmdclass={"build_ext": CustomBuildExt},
)
