//
//  emuone-ibm360/Ibm360Storage.hpp
//
//  IBM/360 storage
//
//////////
#pragma once

class EMUONE_IBM360_EXPORT Ibm360StorageEditor;

//////////
//  The IBM/360 storage
class EMUONE_IBM360_EXPORT Ibm360Storage : public Component
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Ibm360Storage)

    friend class Ibm360StorageEditor;

    //////////
    //  Types
public:
    //  The "type" of IBM/360 mstorage
    class EMUONE_IBM360_EXPORT Type : public ComponentType
    {
        DECLARE_SINGLETON(Type)

        //////////
        //  StockObject
    public:
        virtual QString     getMnemonic() const override;
        virtual QString     getDisplayName() const override;

        //////////
        //  ComponentType
    public:
        virtual ComponentCategory * getCategory() const override;
        virtual bool        isCompatibleWith(Architecture * architecture) const override;
        virtual Ibm360Storage * createComponent() override;
    };

    //////////
    //  Construction/destruction
public:
    Ibm360Storage(const QString & name, const MemorySize & size);
    virtual ~Ibm360Storage();

    //////////
    //  Component
public:
    virtual ComponentType * getType() const override;
    virtual ComponentEditor*createEditor(QWidget * parent) override;
    virtual QString     getShortStatus() const override;

    //////////
    //  Component (state control) - all thread-safe
public:
    virtual State       getState() const override;
    virtual void        connect() override;
    virtual void        initialise() override;
    virtual void        start() override;
    virtual void        stop() noexcept override;
    virtual void        deinitialise() noexcept override;
    virtual void        disconnect() noexcept override;

    //////////
    //  Component (serialisation)
public:
    virtual void        serialiseConfiguration(QDomElement & configurationElement) const override;
    virtual void        deserialiseConfiguration(QDomElement & configurationElement) override;

    //////////
    //  Operations
public:
    static bool         isValidSize(const MemorySize & size);

    MemorySize          getSize() const { return _size; }
    bool                setSize(const MemorySize & size);

    //////////
    //  Implementation
private:
    State               _state = State::Constructed;
    mutable QRecursiveMutex _stateGuard;

    //  Component configuration
    MemorySize          _size;
};

//  End of emuone-ibm360/Ibm360Storage.hpp
