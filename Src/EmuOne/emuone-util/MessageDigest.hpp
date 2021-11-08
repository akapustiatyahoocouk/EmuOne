//
//  util/MessageDigest.hpp
//
//  Message digest support
//
//////////
#pragma once

namespace util
{
    class EMUONE_UTIL_EXPORT MessageDigest;
    using MessageDigestList = QList<MessageDigest*>;

    //////////
    //  A generic "message digest" - an algorithm that creates a fixed-size "digest"
    //  for an arbitrary size message
    class EMUONE_UTIL_EXPORT MessageDigest : public StockObject
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MessageDigest)

        //////////
        //  Types
    public:
        //  The "builder" of a message digest.
        //  Once created, a Builder is fed the message bytes (all at once or piecemeal),
        //  after which it is "finalised". Once "finalised", the digest bytes become available.
        class EMUONE_UTIL_EXPORT Builder
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Builder)

            //////////
            //  Construction/destruction
        public:
            Builder() {}
            virtual ~Builder() {}

            //////////
            //  Operations
        public:
            //  The MessageDigest implemented by this Builder.
            virtual MessageDigest * messageDigest() const = 0;

            //  Resets this Builder to its initial state, ready to digest the next message
            virtual void        reset() = 0;

            //  Modifies the state of this builder by processing the specified bytes.
            virtual void        digestBytes(const void * data, size_t numBytes) = 0;

            //  Finalises the state of this builder and calculates the "final" message digest.
            virtual void        finalise() = 0;

            //  Returns the "final" message digest; "finalise()"s first if necessary.
            virtual QByteArray  digest() = 0;

            //  Same as "digest()", but returns a string of [uppercase] hex digits, two per byte.
            virtual QString     digestString();
        };

        //////////
        //  Construction/destruction - from derived classes only
    protected:
        MessageDigest();
        virtual ~MessageDigest();

        //////////
        //  StockObject
    public:
        virtual QIcon           smallIcon() const override;
        virtual QIcon           largeIcon() const override;

        //////////
        //  Operations
    public:
        //  Creates a new Builder that implements this MessageDigest.
        //  The caller is responsible for destroying the Builder when done.
        virtual Builder *       createBuilder() = 0;

        //////////
        //  Registry
    public:
        static MessageDigestList all();
        static MessageDigest *  findByMnemonic(const QString & mnemonic);
        static bool             registerMessageDigest(MessageDigest * messageDigest);
    private:
        static MessageDigestList _registry;

        //////////
        //  Implementation
    private:
        mutable QIcon           _smallIcon;
        mutable QIcon           _largeIcon;
    };

    //////////
    //  SHA-1 message digest
    class EMUONE_UTIL_EXPORT Sha1MessageDigest : public MessageDigest
    {
        DECLARE_SINGLETON(Sha1MessageDigest)

        //////////
        //  StockObject
    public:
        virtual QString         mnemonic() const override;
        virtual QString         displayName() const override;

        //////////
        //  MessageDigest
    public:
        virtual Builder *       createBuilder() override;

        //////////
        //  Implementation
    private:
        class EMUONE_UTIL_EXPORT _Builder : public Builder
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Builder)

            //////////
            //  Construction/destruction
        public:
            _Builder();
            virtual ~_Builder();

            //////////
            //  Builder
        public:
            virtual MessageDigest * messageDigest() const override;
            virtual void        reset() override;
            virtual void        digestBytes(const void * data, size_t numBytes) override;
            virtual void        finalise() override;
            virtual QByteArray  digest() override;

            //////////
            //  Implementation
        private:
            bool                _finalised = false;

            uint32_t            _H[5];      //  Message digest buffers
            uint32_t            _lengthLo;  //  Message length in bits
            uint32_t            _lengthHi;  //  Message length in bits

            uint8_t             _messageBlock[64];  //  512-bit message blocks
            int                 _messageBlockIndex; //  Index into message block array

            QByteArray          _result;    //  empty unless finalized

            //  Helpers
            void                _processMessageBlock();
            void                _padMessage();
            inline uint32_t     _circularShift(int bits, uint32_t word) { return (word << bits) | (word >> (32 - bits)); }
        };
    };
}

//  End of util/MessageDigest.hpp
