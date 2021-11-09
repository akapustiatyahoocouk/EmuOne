//
//  emuone-core/StandardMemory.hpp
//
//  The standard memory components
//
//////////
#pragma once
#include "emuone-core/API.hpp"
#include <QWidget>

namespace core
{
    //////////
    //  Traits of a "memory address" data type.
    //  Specialisations of this template define various properties of a
    //  "memory address<T>" data type of various width.
    template <int numBits>
    struct AddressTraits
    {   //  ...it's all in the specialisations!
    };

    template <>
    struct AddressTraits<16>
    {   //  Traits of 16-bit address
        typedef uint16_t        DataType;   //  ...used to represent addresses
        inline static DataType  normalise(DataType address) { return address; }
    };

    template <>
    struct AddressTraits<24>
    {   //  Traits of 24-bit address
        typedef uint32_t        DataType;   //  ...used to represent addresses
        inline static DataType  normalise(DataType address) { return address & UINT32_C(0x00FFFFFF); }
    };

    template <>
    struct AddressTraits<32>
    {   //  Traits of 32-bit address
        typedef uint32_t        DataType;   //  ...used to represent addresses
        inline static DataType  normalise(DataType address) { return address; }
    };

    template <>
    struct AddressTraits<64>
    {   //  Traits of 64-bit address
        typedef uint64_t        DataType;   //  ...used to represent addresses
        inline static DataType  normalise(DataType address) { return address; }
    };

    //////////
    //  Traits of a "value in memory" data type.
    //  Specialisations of this template define various properties of a
    //  "value in memory<T>" data type of various width.
    template <int numBits>
    struct ValueTraits
    {   //  ...it's all in the specialisations!
    };

    template <>
    struct ValueTraits<8>
    {   //  Traits of a 8-bit (byte) value
        typedef uint8_t         DataType;   //  ...used to represent data values
        inline static DataType  normalise(DataType address) { return address; }

        //  Operations
        inline static void      copy(DataType * to, const DataType * from, size_t count)
        {
            memcpy(to, from, count * sizeof(DataType));
        }
    };

    //////////
    //  A memory unit that uses addresses "addressBits" bits long, and stores a value
    //  that is "dataBits" bits long at each address
    template<int addressBits, int dataBits>
    class MemoryUnit : public Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MemoryUnit)

        //////////
        //  Constants
    public:
        static const int AddressBits = addressBits; //  the number of bits in an addess
        static const int DataBits = dataBits;       //  the number of bits in a data item at each address

        //////////
        //  Types
    public:
        //  The data type used to represent addresses
        typedef typename AddressTraits<addressBits>::DataType AddressType;

        //  The data type used to represent data values at each address
        typedef typename ValueTraits<dataBits>::DataType DataType;

        //  Possible outcomes of a memory access attempt
        enum class AccessOutcome
        {
            Success,            //  memory access succeeded
            InvalidAddress,     //  address is e.g. out of range
            InvalidAlignment,   //  address is e.g. mis-aligned
            AccessDenied,       //  access not wllowed (e.g. writing to ROM, etc.)
            UnknownError        //  unknown / hardware error
        };

        //////////
        //  Construction/destruction
    public:
        MemoryUnit(const QString & name,
                   typename AddressTraits<addressBits>::DataType startAddress,
                   const MemorySize & size);
        virtual ~MemoryUnit();

        //////////
        //  Operations (configuration)
    public:
        AddressType             startAddress() const { return _startAddress; }
        void                    setStartAddress(AddressType startAddress) { _startAddress = AddressTraits<addressBits>::normalise(startAddress); }
        AddressType             endAddress() const { return AddressTraits<addressBits>::normalise(_startAddress + (_size.toBytes() - 1)); }
        MemorySize              size() const { return _size; }
        void                    setSize(const MemorySize & size) { _size = size; }

        //////////
        //  Operations (access)
    public:
        //  Loads a single data item from the specified "address" into the "value".
        //  Returns the outcome - unless "Success", the "value" is undefined.
        virtual AccessOutcome   load(AddressType address, DataType & value) const = 0;

        //  stores a single data item to the specified "address" from the "value".
        //  Returns the outcome - unless "Success", the "value" is not guaranteed to have been stored.
        virtual AccessOutcome   store(AddressType address, DataType value) = 0;

        //  Loads consecutive data items from the specified "address" into the "values".
        //  Returns the outcome - unless "Success", the "values" is not guaranteed to be consistent.
        virtual AccessOutcome   loadMultiple(AddressType address, DataType * values, size_t numValues) const = 0;

        //  Stores consecutive data items to the specified "address" from the "values".
        //  Returns the outcome - unless "Success", the "values" is not guaranteed to have been all stored.
        virtual AccessOutcome   storeMultiple(AddressType address, const DataType * values, size_t numValues) = 0;

        //////////
        //  Implementation
    private:
        //  Configuration
        AddressType             _startAddress;
        MemorySize              _size;
    };

    template<int addressBits, int dataBits>
    MemoryUnit<addressBits, dataBits>::MemoryUnit(const QString & name,
                                                  typename AddressTraits<addressBits>::DataType startAddress,
                                                  const MemorySize & size)
        :   Component(name),
            _startAddress(startAddress),
            _size(size)
    {
        Q_ASSERT(_size.toBytes() != 0);
    }

    template<int addressBits, int dataBits>
    MemoryUnit<addressBits, dataBits>::~MemoryUnit()
    {
    }

    //////////
    ///
    //  A "memory unit" whose content is kept entirely in the host RAM
    template<int addressBits, int dataBits>
    class ResidentMemoryUnit : public MemoryUnit<addressBits, dataBits>
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ResidentMemoryUnit)

        //////////
        //  Types
    public:
        typedef typename AddressTraits<addressBits>::DataType AddressType;
        typedef typename ValueTraits<dataBits>::DataType DataType;
        typedef typename MemoryUnit<addressBits, dataBits>::AccessOutcome AccessOutcome;

        //////////
        //  Construction/destruction
    public:
        ResidentMemoryUnit(const QString & name,
                           typename AddressTraits<addressBits>::DataType startAddress,
                           const MemorySize & size);
        virtual ~ResidentMemoryUnit();

        //////////
        //  Component (state control) - all thread-safe
    public:
        virtual Component::State    state() const override;
        virtual void            connect() override;
        virtual void            initialise() override;
        virtual void            start() override;
        virtual void            stop() noexcept override;
        virtual void            deinitialise() noexcept override;
        virtual void            disconnect() noexcept override;

        //////////
        //  Operations (access)
    public:
        virtual AccessOutcome   load(AddressType address, DataType & value) const override;
        virtual AccessOutcome   store(AddressType address, DataType value) override;
        virtual AccessOutcome   loadMultiple(AddressType address, DataType * values, size_t numValues) const override;
        virtual AccessOutcome   storeMultiple(AddressType address, const DataType * values, size_t numValues) override;

        //////////
        //  Implementation
    private:
        Component::State        _state = Component::State::Constructed;
        mutable QRecursiveMutex _stateGuard;

        //  Runtime state
        DataType *              _content = nullptr; //  array of "_contentSize" elements
        size_t                  _contentSize = 0;
    };

    template<int addressBits, int dataBits>
    ResidentMemoryUnit<addressBits, dataBits>::ResidentMemoryUnit(const QString & name,
                                                                  typename AddressTraits<addressBits>::DataType startAddress,
                                                                  const MemorySize & size)
        :   MemoryUnit<addressBits, dataBits>(name, startAddress, size)
    {
    }

    template<int addressBits, int dataBits>
    ResidentMemoryUnit<addressBits, dataBits>::~ResidentMemoryUnit()
    {
        delete [] _content; //  ...just in case
    }

    template<int addressBits, int dataBits>
    Component::State ResidentMemoryUnit<addressBits, dataBits>::state() const
    {
        QMutexLocker lock(&_stateGuard);
        return _state;
    }

    template<int addressBits, int dataBits>
    void ResidentMemoryUnit<addressBits, dataBits>::connect()
    {
        QMutexLocker lock(&_stateGuard);

        if (_state != Component::State::Constructed)
        {   //  OOPS! Can't make this state transiton!
            return;
        }

        //  Done
        _state = Component::State::Connected;
    }

    template<int addressBits, int dataBits>
    void ResidentMemoryUnit<addressBits, dataBits>::initialise()
    {
        QMutexLocker lock(&_stateGuard);

        if (_state != Component::State::Connected)
        {   //  OOPS! Can't make this state transiton!
            return;
        }

        if ((size_t)this->size().toBytes() != this->size().toBytes())
        {   //  OOPS! Size overflow!
            throw VirtualApplianceException("Resident memory unit too large");
        }

        try
        {
            size_t numItems = (size_t)this->size().toBytes();
            _content = new DataType[numItems];
            _contentSize = numItems;
            for (size_t i = 0; i < numItems; i++)
            {
                _content[i] = 0;
            }
        }
        catch (...)
        {   //  OOPS! Not enough host RAM! "_data" remains "nullptr".
            throw VirtualApplianceException("Resident memory unit too large");
        }

        //  Done
        _state = Component::State::Initialised;
    }

    template<int addressBits, int dataBits>
    void ResidentMemoryUnit<addressBits, dataBits>::start()
    {
        QMutexLocker lock(&_stateGuard);

        if (_state != Component::State::Initialised)
        {   //  OOPS! Can't make this state transiton!
            return;
        }

        //  Done
        _state = Component::State::Running;
    }

    template<int addressBits, int dataBits>
    void ResidentMemoryUnit<addressBits, dataBits>::stop() noexcept
    {
        QMutexLocker lock(&_stateGuard);

        if (_state != Component::State::Running)
        {   //  OOPS! Can't make this state transiton!
            return;
        }

        //  Done
        _state = Component::State::Initialised;
    }

    template<int addressBits, int dataBits>
    void ResidentMemoryUnit<addressBits, dataBits>::deinitialise() noexcept
    {
        QMutexLocker lock(&_stateGuard);

        if (_state != Component::State::Initialised)
        {   //  OOPS! Can't make this state transiton!
            return;
        }

        delete [] _content;
        _content = nullptr;
        _contentSize = 0;

        //  Done
        _state = Component::State::Connected;
    }

    template<int addressBits, int dataBits>
    void ResidentMemoryUnit<addressBits, dataBits>::disconnect() noexcept
    {
        QMutexLocker lock(&_stateGuard);

        if (_state != Component::State::Connected)
        {   //  OOPS! Can't make this state transiton!
            return;
        }

        //  Done
        _state = Component::State::Constructed;
    }

    template<int addressBits, int dataBits>
    typename MemoryUnit<addressBits, dataBits>::AccessOutcome ResidentMemoryUnit<addressBits, dataBits>::load(AddressType address, DataType & value) const
    {
        if (_content == nullptr)
        {   //  OOPS! Invalid state!
            return AccessOutcome::UnknownError;
        }
        else if (address >= _contentSize)
        {   //  OOPS! Out of range!
            return AccessOutcome::InvalidAddress;
        }
        else
        {   //  Access!
            value = _content[address];
            return AccessOutcome::Success;
        }
    }

    template<int addressBits, int dataBits>
    typename MemoryUnit<addressBits, dataBits>::AccessOutcome ResidentMemoryUnit<addressBits, dataBits>::store(AddressType address, DataType value)
    {
        if (_content == nullptr)
        {   //  OOPS! Invalid state!
            return AccessOutcome::UnknownError;
        }
        else if (address >= _contentSize)
        {   //  OOPS! Out of range!
            return AccessOutcome::InvalidAddress;
        }
        else
        {   //  Access!
            _content[address] = value;
            return AccessOutcome::Success;
        }
    }

    template<int addressBits, int dataBits>
    typename MemoryUnit<addressBits, dataBits>::AccessOutcome ResidentMemoryUnit<addressBits, dataBits>::loadMultiple(AddressType address, DataType * values, size_t numValues) const
    {
        if (_content == nullptr)
        {   //  OOPS! Invalid state!
            return AccessOutcome::UnknownError;
        }
        else if (address >= _contentSize || numValues > _contentSize || address + _contentSize > address)
        {   //  OOPS! Out of range!
            return AccessOutcome::InvalidAddress;
        }
        else
        {   //  Access!
            ValueTraits<dataBits>::copy(values, _content + address, numValues);
            return AccessOutcome::Success;
        }
    }

    template<int addressBits, int dataBits>
    typename MemoryUnit<addressBits, dataBits>::AccessOutcome ResidentMemoryUnit<addressBits, dataBits>::storeMultiple(AddressType address, const DataType * values, size_t numValues)
    {
        if (_content == nullptr)
        {   //  OOPS! Invalid state!
            return AccessOutcome::UnknownError;
        }
        else if (address >= _contentSize || numValues > _contentSize || address + _contentSize > address)
        {   //  OOPS! Out of range!
            return AccessOutcome::InvalidAddress;
        }
        else
        {   //  Access!
            ValueTraits<dataBits>::copy(_content + address, values, numValues);
            return AccessOutcome::Success;
        }
    }





/*  TODO kill off
    //////////
    //  The standard "64-bit memory unit" covers a continuous rangel of a 64-bit address space
    class EMUONE_CORE_EXPORT StandardMemoryUnit64 : public Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(StandardMemoryUnit64)

        //////////
        //  Construction/destruction
    public:
        StandardMemoryUnit64(const QString & name, uint64_t startAddress, const MemorySize & size);
        virtual ~StandardMemoryUnit64();

        //////////
        //  Component (serialisation)
    public:
        virtual void            serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void            deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  Operations
    public:
        uint64_t                startAddress() const { return _startAddress; }
        void                    setStartAddress(uint64_t startAddress) { _startAddress = startAddress; }
        uint64_t                endAddress() const { return _startAddress + (_size.toBytes() - 1); }
        MemorySize              size() const { return _size; }
        void                    setSize(const MemorySize & size) { _size = size; }

        //////////
        //  Implementation
    private:
        //  Configuration
        uint64_t                _startAddress;
        MemorySize              _size;
    };
    typedef QList<StandardMemoryUnit64*> StandardMemoryUnit64List;

    //////////
    //  The standard "64-bit RAM unit" which resides entirely in host RAM
    class EMUONE_CORE_EXPORT StandardResidentRamUnit64 : public StandardMemoryUnit64
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(StandardResidentRamUnit64)

        friend class ResidentRamUnit64Editor;

        //////////
        //  Construction/destruction
    public:
        StandardResidentRamUnit64(const QString & name, uint64_t startAddress, const MemorySize & size);
        virtual ~StandardResidentRamUnit64();

        //////////
        //  Component
    public:
        virtual ComponentEditor *   createEditor(QWidget * parent) override;
        virtual QString             shortStatus() const override;
        virtual ComponentUi *       createUi() override;

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
        //  Operations (serialisation)
    public:

        //////////
        //  Operations
    public:

        //////////
        //  Implementation
    private:
        State                   _state = State::Constructed;
        mutable QRecursiveMutex _stateGuard;

        ResidentRamUnit64EditorList _editors;   //  ...that have been created so far

        //  Runtime state
        uint8_t *               _data = nullptr;
    };

    //////////
    //  The standard "64-bit ROM unit" which resides entirely in host RAM
    class EMUONE_CORE_EXPORT StandardResidentRomUnit64 : public StandardMemoryUnit64
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(StandardResidentRomUnit64)

        //////////
        //  Construction/destruction
    public:
        StandardResidentRomUnit64(const QString & name, uint64_t startAddress, const MemorySize & size, const QString & contentFileName);
        virtual ~StandardResidentRomUnit64();

        //////////
        //  Component
    public:
        virtual ComponentEditor *   createEditor(QWidget * parent) override;
        virtual QString             shortStatus() const override;
        virtual ComponentUi *       createUi() override;

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
        //  Operations (serialisation)
    public:
        virtual void            serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void            deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  Operations
    public:
        QString                 contentFileName() const { return _contentFileName; }
        void                    setContentFileName(const QString & contentFileName) { _contentFileName = contentFileName; }

        //////////
        //  Implementation
    private:
        State                   _state = State::Constructed;
        mutable QRecursiveMutex _stateGuard;

        ResidentRomUnit64EditorList _editors;   //  ...that have been created so far

        //  Configuration
        QString                 _contentFileName;

        //  Runtime state
        uint8_t *               _data = nullptr;
        size_t                  _dataSize = 0;
    };

    //////////
    //  The standard "64-bit memory bus" component.
    //  Allows any number of "StandardMemoryUnit64"s to be "attached" as long as
    //  their recognised address ranges do not overlap.
    class EMUONE_CORE_EXPORT StandardMemoryBus64 : public Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(StandardMemoryBus64)

        //////////
        //  Construction/destruction
    public:
        explicit StandardMemoryBus64(const QString & name);
        virtual ~StandardMemoryBus64();

        //////////
        //  Component
    public:
        virtual ComponentEditor *   createEditor(QWidget * parent) override;
        virtual QString             shortStatus() const override;
        virtual ComponentUi *       createUi() override;

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
        //  Operations (serialisation)
    public:
        virtual void            serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void            deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  Operations
    public:
        //  Attaches the specified "memoryUnit" to this Memory Bus.
        //  Throws VirtualApplianceException if an error occurs (e.g. address overlap).
        virtual void            attachMemoryUnit(StandardMemoryUnit64 * memoryUnit);

        //  Detaches the specified "memoryUnit" from this Memory Bus.
        virtual void            detachMemoryUnit(StandardMemoryUnit64 * memoryUnit);

        //////////
        //  Hooks to be overridden in concrete derived classes
    protected:
        virtual StandardMemoryUnit64List    findMemoryUnits() = 0;

        //////////
        //  Implementation
    private:
        State                   _state = State::Constructed;
        mutable QRecursiveMutex _stateGuard;

        //  Runtime state
        size_t                  _numMemoryUnits = 0;
        StandardMemoryUnit64 ** _memoryUnits = nullptr; //  array of _numMemoryUnits, sorted by "startAddress"
    };
*/
}

//  End of emuone-core/StandardMemory.hpp
