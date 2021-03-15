#ifndef __BLOCKDATA_HPP__
#define __BLOCKDATA_HPP__

#include <condition_variable>
#include <future>
#include <memory>
#include <mutex>
#include <stdint.h>
#include <stdio.h>
#include <vector>

//#include "Bitmap.hpp"
#include "ForceInline.hpp"
#include "Vector.hpp"

enum class Channels
{
    RGB,
    Alpha
};

class BlockData
{
public:
    enum Type
    {
        Etc1,
        Etc2_RGB,
        Etc2_RGBA,
        Dxt1,
        Dxt5
    };

    BlockData( const char* fn );
    BlockData( const char* fn, const v2i& size, bool mipmap, Type type );
    BlockData( const v2i& size, bool mipmap, Type type );
    ~BlockData();

    uint8_t* Decode();

    void Process( const uint32_t* src, uint32_t blocks, size_t offset, size_t width, Channels type, bool dither );
    void ProcessRGBA( const uint32_t* src, uint32_t blocks, size_t offset, size_t width );

    const v2i& Size() const { return m_size; }

    static uint32_t* PubDecodeDxt1(uint64_t* src, uint32_t width, uint32_t height);
    static uint32_t* PubDecodeDxt5(uint64_t* src, uint32_t width, uint32_t height);
    static uint32_t* PubDecodeETCRGB(uint64_t* src, uint32_t width, uint32_t height);
    static uint32_t* PubDecodeETCRGBA(uint64_t* src, uint32_t width, uint32_t height);

private:
    etcpak_no_inline uint8_t* DecodeRGB();
    etcpak_no_inline uint8_t* DecodeRGBA();
    etcpak_no_inline uint8_t* DecodeDxt1();
    etcpak_no_inline uint8_t* DecodeDxt5();

    uint8_t* m_data;
    v2i m_size;
    size_t m_dataOffset;
    FILE* m_file;
    size_t m_maplen;
    Type m_type;
};

typedef std::shared_ptr<BlockData> BlockDataPtr;
#endif
