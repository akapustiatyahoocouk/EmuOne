//
//  emuone-util/CharacterSets.hpp
//
//  Character Set support
//
//////////

namespace util
{
    class EMUONE_UTIL_EXPORT CharacterSet;
    using CharacterSetList = QList<CharacterSet*>;

    //////////
    //  A generic "character set" defnes bi-directional translation
    //  between sequences of characters and sequences of bytes
    class EMUONE_UTIL_EXPORT CharacterSet : public StockObject
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CharacterSet)

        //////////
        //  Types
    public:
        //  A generic bytes/characters convertor
        class EMUONE_UTIL_EXPORT Transcoder
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Transcoder)

            //////////
            //  Construction/destruction
        public:
            Transcoder() {}
            virtual ~Transcoder() {}

            //////////
            //  Operations
        public:
            //  The CharacterSet implemented by this Transcoder
            virtual CharacterSet *  characterSet() const = 0;

            //  Resets this transcoder to its initial (post-construction) state
            virtual void        reset() = 0;
        };

        //  Converts character data to byte data
        class EMUONE_UTIL_EXPORT Encoder : public Transcoder
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Encoder)

            //////////
            //  Construction/destruction
        public:
            Encoder() {}
            virtual ~Encoder() {}

            //////////
            //  Operations
        public:
            //  Checks if one or more bytes starting from "bytes[scan]" can be
            //  decoded into a single character.
            virtual bool        canEncodeCharacter(QChar c) { return encodingLength(c) > 0; }

            //  The number of bytes it would require for this Encoder in its
            //  current state to encode the specified character; 0 == can't do it.
            virtual int         encodingLength(QChar c) = 0;

            //  If the specified character can be encoded by this Encoder in its
            //  current state, appends the encoding bytes to "bytes" and returns
            //  the number of bytes appended; otherwise returns 0.
            virtual int         encode(QChar c, QByteArray & bytes) = 0;

            //  Encodes all characters of "s", appending the encoded bytes
            //  repeatedly to the end of "bytes"; returns the total number of bytes appended.
            virtual int         encode(const QString & s, QByteArray & bytes) = 0;
        };

        //  Converts byte data to character data
        class EMUONE_UTIL_EXPORT Decoder : public Transcoder
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Decoder)

            //////////
            //  Construction/destruction
        public:
            Decoder() {}
            virtual ~Decoder() {}

            //////////
            //  Operations
        public:
            virtual bool        canDecodeCharacter(const QByteArray & bytes, int scan) { return encodingLength(bytes, scan) > 0; }

            //  Returns the number of bytes that can be decoded into a single character from
            //  offset "scan" in "bytes". Returns 0 is no bytes can be so decoded (i.e.
            //  "bytes[scan...]" is not a valid encoding, or there are too few bytes
            //  in "bytes" and more data is needed.
            virtual int         encodingLength(const QByteArray & bytes, int scan) = 0;

            //  Decodes a single character from offset "scan" in "bytes" into "c".
            //  Returns the number of bytes consumed or 0 if a character could not be decoded.
            virtual int         decode(const QByteArray & bytes, int scan, QChar & c) = 0;

            //  Decodes as many characters as possible from offset "scan" in "bytes" into "s".
            //  Returns the total number of bytes consumed or 0 if nothing could not be decoded.
            virtual int         decode(const QByteArray & bytes, int scan, QString & s) = 0;
        };

        //////////
        //  Construction/destruction
    public:
        CharacterSet();
        virtual ~CharacterSet();

        //////////
        //  StockObject
    public:
        virtual QIcon           smallIcon() const override;
        virtual QIcon           largeIcon() const override;

        //////////
        //  Operations
    public:
        //  The minimum/maximum number of bytes used for encoding a single
        //  character using this CharacterSet
        virtual int             minimumEncodingLength() const = 0;
        virtual int             maximumEncodingLength() const = 0;

        //  A character that is guaranteed to be encodable using this CharacterSet.
        //  In some operation modes it will be used to replace a non-encodable
        //  character
        virtual QChar           defaultCharacter() const { return '?'; }

        //  Creates an Encoder/Decoder that implements this CharacterSet.
        //  The caller is responsible for "delete"-int it when done.
        virtual Encoder *       createEncoder() = 0;
        virtual Decoder *       createDecoder() = 0;

        //////////
        //  Registry
    public:
        static CharacterSetList all();
        static CharacterSet *   findByMnemonic(const QString & mnemonic);
        static bool             registerCharacterSet(CharacterSet * Charactercet);
    private:
        static CharacterSetList _registry;

        //////////
        //  Implementation
    private:
        mutable QIcon           _smallIcon = QIcon();
        mutable QIcon           _largeIcon = QIcon();
    };

    //////////
    //  The ISO-8859-1 character set
    class EMUONE_UTIL_EXPORT Iso88591CharacterSet : public CharacterSet
    {
        DECLARE_SINGLETON(Iso88591CharacterSet)

        //////////
        //  StockObject
    public:
        virtual QString         mnemonic() const override;
        virtual QString         displayName() const override;

        //////////
        //  CharacterSet
    public:
        virtual int             minimumEncodingLength() const override;
        virtual int             maximumEncodingLength() const override;
        virtual Encoder *       createEncoder() override;
        virtual Decoder *       createDecoder() override;

        //////////
        //  Implementation
    private:
        class EMUONE_UTIL_EXPORT _Encoder : public Encoder
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Encoder)

            //////////
            //  Construction/destruction
        public:
            _Encoder() {}
            virtual ~_Encoder() {}

            //////////
            //  Transcoder
        public:
            virtual CharacterSet *  characterSet() const override;
            virtual void            reset() override;

            //////////
            //  Encoder
        public:
            virtual int         encodingLength(QChar c) override;
            virtual int         encode(QChar c, QByteArray & bytes) override;
            virtual int         encode(const QString & s, QByteArray & bytes) override;
        };

        class EMUONE_UTIL_EXPORT _Decoder : public Decoder
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Decoder)

            //////////
            //  Construction/destruction
        public:
            _Decoder() {}
            virtual ~_Decoder() {}

            //////////
            //  Transcoder
        public:
            virtual CharacterSet *  characterSet() const override;
            virtual void        reset() override;

            //////////
            //  Decoder
        public:
            virtual int         encodingLength(const QByteArray & bytes, int scan) override;
            virtual int         decode(const QByteArray & bytes, int scan, QChar & c) override;
            virtual int         decode(const QByteArray & bytes, int scan, QString & s) override;
        };
    };

    //////////
    //  The IBM EBCDIC (US) character set
    class EMUONE_UTIL_EXPORT Cp037CharacterSet : public CharacterSet
    {
        DECLARE_SINGLETON(Cp037CharacterSet)

        //////////
        //  StockObject
    public:
        virtual QString         mnemonic() const override;
        virtual QString         displayName() const override;

        //////////
        //  CharacterSet
    public:
        virtual int             minimumEncodingLength() const override;
        virtual int             maximumEncodingLength() const override;
        virtual Encoder *       createEncoder() override;
        virtual Decoder *       createDecoder() override;

        //////////
        //  Implementation
    private:
        static const uint8_t    _ebcdicToAscii[256];
        static uint8_t          _asciiToEbcdic[256];

        class EMUONE_UTIL_EXPORT _Encoder : public Encoder
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Encoder)

            //////////
            //  Construction/destruction
        public:
            _Encoder() {}
            virtual ~_Encoder() {}

            //////////
            //  Transcoder
        public:
            virtual CharacterSet *  characterSet() const override;
            virtual void        reset() override;

            //////////
            //  Encoder
        public:
            virtual int         encodingLength(QChar c) override;
            virtual int         encode(QChar c, QByteArray & bytes) override;
            virtual int         encode(const QString & s, QByteArray & bytes) override;
        };

        class EMUONE_UTIL_EXPORT _Decoder : public Decoder
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Decoder)

            //////////
            //  Construction/destruction
        public:
            _Decoder() {}
            virtual ~_Decoder() {}

            //////////
            //  Transcoder
        public:
            virtual CharacterSet *  characterSet() const override;
            virtual void        reset() override;

            //////////
            //  Decoder
        public:
            virtual int         encodingLength(const QByteArray & bytes, int scan) override;
            virtual int         decode(const QByteArray & bytes, int scan, QChar & c) override;
            virtual int         decode(const QByteArray & bytes, int scan, QString & s) override;
        };
    };
}

//  End of emuone-util/CharacterSets.hpp
