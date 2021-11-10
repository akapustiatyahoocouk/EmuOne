//
//  emuone-util/Memory.hpp
//
//  Memory access helpers
//
//////////

namespace util
{
    //////////
    //  Byte orders
    enum class ByteOrder
    {
        BigEndian,
        LittleEndian,
        //  The host CPY byte order
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
        Host = BigEndian
#elif Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        Host = LittleEndian
#else
    #error Unsupported byte order
#endif
    };

    inline uint16_t changeByteOrder(uint16_t x) { return __builtin_bswap16(x); }
    inline uint32_t changeByteOrder(uint32_t x) { return __builtin_bswap32(x); }
    inline uint64_t changeByteOrder(uint64_t x) { return __builtin_bswap64(x); }
    inline int16_t  changeByteOrder(int16_t x)  { return __builtin_bswap16(x); }
    inline int32_t  changeByteOrder(int32_t x)  { return __builtin_bswap32(x); }
    inline int64_t  changeByteOrder(int64_t x)  { return __builtin_bswap64(x); }

    //////////
    //  Host memory access services
    class EMUONE_UTIL_EXPORT Memory
    {
        UTILITY_CLASS(Memory)

        //////////
        //  Operations
    public:
        static uint8_t      loadUInt8(const void * p)
        {
            const uint8_t * bytes = (const uint8_t *)p;
            return *bytes;
        }

        static uint16_t     loadUInt16(const void * p, ByteOrder byteOrder)
        {
            const uint16_t * bytes = (const uint16_t *)p;
            return (byteOrder == ByteOrder::Host) ? (*bytes) : changeByteOrder(*bytes);
        }

        static uint32_t     loadUInt32(const void * p, ByteOrder byteOrder)
        {
            const uint32_t * bytes = (const uint32_t *)p;
            return (byteOrder == ByteOrder::Host) ? (*bytes) : changeByteOrder(*bytes);
        }

        static uint64_t     loadUInt64(const void * p, ByteOrder byteOrder)
        {
            const uint64_t * bytes = (const uint64_t *)p;
            return (byteOrder == ByteOrder::Host) ? (*bytes) : changeByteOrder(*bytes);
        }

        static int8_t       loadInt8(const void * p)
        {
            const int8_t * bytes = (const int8_t *)p;
            return *bytes;
        }

        static int16_t      loadInt16(const void * p, ByteOrder byteOrder)
        {
            const int16_t * bytes = (const int16_t *)p;
            return (byteOrder == ByteOrder::Host) ? (*bytes) : changeByteOrder(*bytes);
        }

        static int32_t      loadInt32(const void * p, ByteOrder byteOrder)
        {
            const int32_t * bytes = (const int32_t *)p;
            return (byteOrder == ByteOrder::Host) ? (*bytes) : changeByteOrder(*bytes);
        }

        static int64_t      loadInt64(const void * p, ByteOrder byteOrder)
        {
            const int64_t * bytes = (const int64_t *)p;
            return (byteOrder == ByteOrder::Host) ? (*bytes) : changeByteOrder(*bytes);
        }

        static void         storeUInt8(const void * p, uint8_t value);
        static void         storeUInt16(const void * p, uint16_t value, ByteOrder byteOrder);
        static void         storeUInt32(const void * p, uint32_t value, ByteOrder byteOrder);
        static void         storeUInt64(const void * p, uint64_t value, ByteOrder byteOrder);
        static void         storeInt8(const void * p, int8_t value);
        static void         storeInt16(const void * p, int16_t value, ByteOrder byteOrder);
        static void         storeInt32(const void * p, int32_t value, ByteOrder byteOrder);
        static void         storeInt64(const void * p, int64_t value, ByteOrder byteOrder);

        static uint8_t      loadUnalignedUInt8(const void * p);
        static uint16_t     loadUnalignedUInt16(const void * p, ByteOrder byteOrder);
        static uint32_t     loadUnalignedUInt32(const void * p, ByteOrder byteOrder);
        static uint64_t     loadUnalignedUInt64(const void * p, ByteOrder byteOrder);
        static int8_t       loadUnalignedInt8(const void * p);
        static int16_t      loadUnalignedInt16(const void * p, ByteOrder byteOrder);
        static int32_t      loadUnalignedInt32(const void * p, ByteOrder byteOrder);
        static int64_t      loadUnalignedInt64(const void * p, ByteOrder byteOrder);

        static void         storeUnalignedUInt8(const void * p, uint8_t value);
        static void         storeUnalignedUInt16(const void * p, uint16_t value, ByteOrder byteOrder);
        static void         storeUnalignedUInt32(const void * p, uint32_t value, ByteOrder byteOrder);
        static void         storeUnalignedUInt64(const void * p, uint64_t value, ByteOrder byteOrder);
        static void         storeUnalignedInt8(const void * p, int8_t value);
        static void         storeUnalignedInt16(const void * p, int16_t value, ByteOrder byteOrder);
        static void         storeUnalignedInt32(const void * p, int32_t value, ByteOrder byteOrder);
        static void         storeUnalignedInt64(const void * p, int64_t value, ByteOrder byteOrder);
    };
}

//  End of emuone-util/Memory.hpp
