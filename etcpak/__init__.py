try:
    from ._etcpak_simd import *  # noqa: F403
except ImportError:
    from ._etcpak import *

# legacy mappings for backwards compatibility
compress_to_dxt1 = compress_bc1  # noqa: F405
compress_to_dxt1_dither = compress_bc1_dither  # noqa: F405
compress_to_dxt5 = compress_bc3  # noqa: F405
compress_to_etc1 = compress_etc1_rgb  # noqa: F405
compress_to_etc1_dither = compress_etc1_rgb_dither  # noqa: F405
compress_to_etc2 = compress_etc2_rgb  # noqa: F405
compress_to_etc2_rgba = compress_etc2_rgba  # noqa: F405
decode_dxt1 = decompress_bc1  # noqa: F405
decode_dxt5 = decompress_bc3  # noqa: F405
decode_etc_rgba = decompress_etc2_rgba  # noqa: F405


def compress_to_etc1_alpha(data: bytes, width: int, height: int) -> bytes:
    raise NotImplementedError("This function was removed in etcpak 0.9.9")


def compress_to_etc2_alpha(data: bytes, width: int, height: int) -> bytes:
    raise NotImplementedError("This function was removed in etcpak 0.9.9")


def set_use_heuristics(use_heuristics: bool) -> None:
    raise NotImplementedError("This function was removed in etcpak 0.9.9")


def get_use_multithreading() -> bool:
    raise NotImplementedError("This function was removed in etcpak 0.9.9")
