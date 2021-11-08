//
//  emuone-util/Memory.hpp
//
//  Memory access helpers
//
//////////

namespace util
{
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
}

//  End of emuone-util/Memory.hpp
