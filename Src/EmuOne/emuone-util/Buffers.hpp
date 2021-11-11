//
//  util/Buffers.hpp
//
//  Buffer support
//
//////////
#pragma once

namespace util
{
    //////////
    //  A generic "buffer" - a sequence of bytes of a known length
    class EMUONE_UTIL_EXPORT Buffer
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Buffer)

        //////////
        //  Construction/destruction
    public:
        Buffer()  {}
        virtual ~Buffer() {}

        //////////
        //  Operations
    public:
        virtual size_t      size() const = 0;
        virtual uint8_t     at(size_t index) const = 0;
        virtual void        setAt(size_t index, uint8_t value) = 0;
    };

    //////////
    //  A "buffer" that wraps a QByteArray
    class EMUONE_UTIL_EXPORT ByteArrayBuffer : public Buffer
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ByteArrayBuffer)

        //////////
        //  Construction/destruction
    public:
        ByteArrayBuffer() : data() {}
        virtual ~ByteArrayBuffer() {}

        //////////
        //  Buffer
    public:
        virtual size_t      size() const override { return data.size(); }
        virtual uint8_t     at(size_t index) const override { return data[index]; }
        virtual void        setAt(size_t index, uint8_t value) override { data[index] = value; }

        //////////
        //  Properties
    public:
        QByteArray          data;
    };
}

//  End of util/Buffers.hpp
