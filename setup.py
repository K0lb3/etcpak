from __future__ import annotations

import os
from typing import List

from archspec.cpu import host
from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext
from wheel.bdist_wheel import bdist_wheel

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

ETCPAK_HEADERS = [
    "b7enc.h",
    "bcdec.h",
    "Bitmap.hpp",
    "BitmapDownsampled.hpp",
    "BlockData.hpp",
    "ColorSpace.hpp",
    "DataProvider.hpp",
    "Debug.hpp",
    "Dither.hpp",
    "Error.hpp",
    "ForceInline.hpp",
    "Math.hpp",
    "MipMap.hpp",
    "mmap.hpp",
    "ProcessCommon.hpp",
    "ProcessDxtc.hpp",
    "ProcessRGB.hpp",
    "Semaphore.hpp",
    "System.hpp",
    "Tables.hpp",
    "TaskDispatch.hpp",
    "Timing.hpp",
    "Vector.hpp",
    # lz4
    "lz4/lz4.h",
    # png
    "libpng/png.h",
    "libpng/pngconf.h",
]

class BuildConfig:
    __SSE4_1__: bool = False
    __AVX2__: bool = False
    __AVX512BW__: bool = False
    __AVX512VL__: bool = False
    __AVX512F__: bool = False
    __ARM_NEON: bool = False
    msvc_flags: List[str] = []
    unix_flags: List[str] = []

    def __init__(self, **kwargs):
        for key, value in kwargs.items():
            setattr(self, key, value)
    
    def items(self):
        return self.__dict__.items()

configs = {
    "sse41": BuildConfig(
        __SSE4_1__=True,
        msvc_flags=["/arch:SSE4.1"],
        unix_flags=["-msse4.1"]
    ),
    "avx2": BuildConfig(
        __SSE4_1__=True,
        __AVX2__=True,
        msvc_flags=["/arch:AVX2"],
        unix_flags=["-msse4.1", "-mfma", "-mbmi", "-mavx2"]
    ),
    "avx512": BuildConfig(
        __SSE4_1__=True,
        __AVX2__=True,
        __AVX512BW__=True,
        __AVX512VL__=True,
        msvc_flags=["/arch:AVX512"],
        unix_flags=["-msse4.1", "-mfma", "-mbmi", "-mavx2", "-mavx512vl", "-mavx512bw", "-mavx512dq", "-mavx512f"]
    ),
    "neon": BuildConfig(
        __ARM_NEON=True,
    )
}


class CustomBuildExt(build_ext):
    def build_extensions(self):
        extra_compile_args: List[str] = []
        extra_link_args: List[str] = []

        # check for cibuildwheel
        cibuildwheel = os.environ.get("CIBUILDWHEEL", False)

        msvc: bool = False
        if self.compiler.compiler_type == "msvc":
            extra_compile_args = [
                "/std:c++20",
                "/Zc:strictStrings-",
                "/DNOMINMAX",
                "/GL",
            ]
            extra_link_args = ["/LTCG:incremental"]
            msvc = True
        else:
            extra_compile_args = [
                "-std=c++20",
                "-flto",
            ]
            extra_link_args = ["-flto"]

            if not cibuildwheel:
                # do some native optimizations for the current machine
                # can't be used for generic builds
                if "-arm" in self.plat_name or "-aarch64" in self.plat_name:
                    native_arg = "-mcpu=native"
                else:
                    native_arg = "-march=native"
                extra_compile_args.append(native_arg)

        local_host = host()

        if self.plat_name.endswith(("amd64", "x86_64")):
            if cibuildwheel:
                self.extensions.extend(
                    [
                        ETCPAKExtension("none", BuildConfig()),
                        ETCPAKExtension("sse41", configs["sse41"]),
                        ETCPAKExtension("avx2", configs["avx2"]),
                        ETCPAKExtension("avx512", configs["avx512"]),
                    ]
                )
            elif "avx512bw" in local_host.features and "avx512vl" in local_host.features:
                self.extensions.append(ETCPAKExtension("avx512", configs["avx512"]))
            elif "avx2" in local_host.features:
                self.extensions.append(ETCPAKExtension("avx2", configs["avx2"]))
            elif "sse4_1" in local_host.features:
                self.extensions.append(ETCPAKExtension("sse41", configs["sse41"]))

        elif self.plat_name.endswith(("arm64", "aarch64")):
            # TODO: somehow detect neon, sve128, sve256
            # atm assume neon is always available
            self.extensions.append(ETCPAKExtension("neon", configs["neon"]))
        elif self.plat_name.endswith("armv7l"):
            # TODO: detect neon
            pass

        for ext in self.extensions:
            ext: ETCPAKExtension
            ext.extra_compile_args.extend(extra_compile_args)
            ext.extra_link_args.extend(extra_link_args)

            build_config = ext.build_config
            if msvc:
                ext.extra_compile_args.extend(build_config.msvc_flags)
            else:
                ext.extra_compile_args.extend(build_config.unix_flags)
            
            for key, value in build_config.items():
                if key.startswith("__"):
                    if value:
                        ext.define_macros.append((key, None))
                    else:
                        ext.undef_macros.append(key)

        super().build_extensions()



class ETCPAKExtension(Extension):
    build_config: BuildConfig
    _needs_stub: bool = False

    def __init__(self, name: str, build_config: BuildConfig):
        module_name = f"_etcpak_{name}"
        super().__init__(
            f"etcpak.{module_name}",
            sources=[
            "src/pylink.cpp",
            "src/dummy.cpp",
            *[f"src/etcpak/{src}" for src in ETCPAK_SOURCES],
            ],
            depends=[
                "src/pybc7params.hpp",
                *[
                    f"src/etcpak/{header}"
                    for header in ETCPAK_HEADERS
                ],
            ],
            include_dirs=["src/etcpak"],
            language="c++",
            extra_compile_args=[
                "-DNDEBUG",
                "-DNO_GZIP",
            ],
            define_macros=[
                ("Py_LIMITED_API", "0x03070000"),
                ("MODULE_NAME", '"module_name"'),
                ("INIT_FUNC_NAME", f"PyInit_{module_name}"),
                # Mac fix due to .c problem
                ("BCDEC_IMPLEMENTATION", "1"),
            ],
            py_limited_api=True,
        )

        self.build_config = build_config



class bdist_wheel_abi3(bdist_wheel):
    def get_tag(self):
        python, abi, plat = super().get_tag()

        if python.startswith("cp"):
            # on CPython, our wheels are abi3 and compatible back to 3.6
            return "cp37", "abi3", plat

        return python, abi, plat

setup(
    name="etcpak",
    packages=["etcpak"],
    package_data={"etcpak": ["*.py", "*.pyi", "py.typed"]},
    ext_modules=[ETCPAKExtension("none", BuildConfig())],
    cmdclass={"build_ext": CustomBuildExt, "bdist_wheel": bdist_wheel_abi3},
)