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
        virtual int         size() const = 0;
        virtual uint8_t     at(int index) const = 0;
        virtual void        setAt(int index, uint8_t value) = 0;
    };

    //////////
    //  A "buffer" that wraps a QByteArray
    class EMUONE_UTIL_EXPORT ByteArrayBuffer : public Buffer
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ByteArrayBuffer)

        //////////
        //  Construction/destruction
    public:
        ByteArrayBuffer()  {}
        virtual ~ByteArrayBuffer() {}

        //////////
        //  Buffer
    public:
        virtual int         size() const override { return data.size(); }
        virtual uint8_t     at(int index) const override { return data[index]; }
        virtual void        setAt(int index, uint8_t value) override { data[index] = value; }

        //////////
        //  Properties
    public:
        QByteArray          data;
    };
}

//  End of util/Buffers.hpp
