//
//  emuone-ibm360/Storage.hpp
//
//  IBM/360 storage
//
//////////
#pragma once

class EMUONE_IBM360_EXPORT Ibm360StorageEditor;

namespace ibm360
{
    //////////
    //  The IBM/360 storage
    class EMUONE_IBM360_EXPORT Storage : public core::Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Storage)

        friend class ::Ibm360StorageEditor;

        //////////
        //  Types
    public:
        //  The "type" of IBM/360 storage
        class EMUONE_IBM360_EXPORT Type : public core::ComponentType
        {
            DECLARE_SINGLETON(Type)

            //////////
            //  StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;

            //////////
            //  ComponentType
        public:
            virtual core::ComponentCategory *   category() const override;
            virtual bool        isCompatibleWith(core::Architecture * architecture) const override;
            virtual Storage *   createComponent() override;
        };

        //////////
        //  Construction/destruction
    public:
        Storage(const QString & name, const core::MemorySize & size);
        virtual ~Storage();

        //////////
        //  Component
    public:
        virtual core::ComponentType*type() const override;
        virtual ComponentEditor *   createEditor(QWidget * parent) override;
        virtual QString             shortStatus() const override;
        virtual core::ComponentUi * createUi() override;

        //////////
        //  Component (state control) - all thread-safe
    public:
        virtual State           state() const override;
        virtual void            connect() override;
        virtual void            initialise() override;
        virtual void            start() override;
        virtual void            stop() noexcept override;
        virtual void            deinitialise() noexcept override;
        virtual void            disconnect() noexcept override;

        //////////
        //  Component (serialisation)
    public:
        virtual void            serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void            deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  Operations
    public:
        static bool             isValidSize(const core::MemorySize & size);

        core::MemorySize        size() const { return _size; }
        bool                    setSize(const core::MemorySize & size);

        //////////
        //  Implementation
    private:
        State                   _state = State::Constructed;
        mutable QRecursiveMutex _stateGuard;

        //  Component configuration
        core::MemorySize        _size;
    };
}

//  End of emuone-ibm360/Storage.hpp
