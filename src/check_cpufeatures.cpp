// copied from: https://github.com/robbmcleod/cpufeature/blob/master/cpufeature/cpu_x86.c
#include <stdint.h>
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)
#if _WIN32
#include "cpu_x86_windows.c"
#elif defined(__GNUC__) || defined(__clang__)
#include "cpu_x86_linux.c"
#endif
#endif

extern void cpuid(int32_t out[4], int32_t level, int32_t count);

bool detect_OS_AVX(void)
{
    //  Copied from: http://stackoverflow.com/a/22521619/922184

    bool avxSupported = false;

    int cpuInfo[4];
    cpuid(cpuInfo, 1, 0);

    bool osUsesXSAVE_XRSTORE = (cpuInfo[2] & (1 << 27)) != 0;
    bool cpuAVXSuport = (cpuInfo[2] & (1 << 28)) != 0;

    if (osUsesXSAVE_XRSTORE && cpuAVXSuport)
    {
        uint64_t xcrFeatureMask = xgetbv(_XCR_XFEATURE_ENABLED_MASK);
        avxSupported = (xcrFeatureMask & 0x6) == 0x6;
    }

    return avxSupported;
}

bool detect_OS_AVX512(void)
{
    if (!detect_OS_AVX())
        return false;

    uint64_t xcrFeatureMask = xgetbv(_XCR_XFEATURE_ENABLED_MASK);
    return (xcrFeatureMask & 0xe6) == 0xe6;
}

bool check_cpu_features()
{
    int info[4]; // [EAX, EBX, ECX, EDX]
    cpuid(info, 0, 0);
    int nIds = info[0];

    if (nIds < 0x00000007)
    {
        return false;
    }

    cpuid(info, 0x80000000, 0);
    uint32_t nExIds = info[0];

    cpuid(info, 0x00000001, 0);
#ifdef __SSE4_1__
    if ((info[2] & ((int)1 << 19)) == 0)
        return false;
#endif
#ifdef __AVX__
    if (((info[2] & ((int)1 << 28)) == 0) || !detect_OS_AVX())
        return false;
#endif

    cpuid(info, 0x00000007, 0);
#ifdef __AVX2__
    if ((info[1] & ((int)1 << 5)) == 0)
        return false;
#endif
#ifdef __AVX512__
    if (!detect_OS_AVX512())
        return false;
#endif
#ifdef __AVX512BW__
    if ((info[1] & ((int)1 << 30)) == 0)
        return false;
#endif
#ifdef __AVX512VL__
    if ((info[1] & ((int)1 << 31)) == 0)
        return false;
#endif

    return true;
}
