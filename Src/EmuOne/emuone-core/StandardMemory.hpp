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
    //  The standard "64-bit memory unit" covers a continuous rangel of a 64-bit address space
    class EMUONE_CORE_EXPORT MemoryUnit64 : public Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MemoryUnit64)

        //////////
        //  Construction/destruction
    public:
        MemoryUnit64(const QString & name, uint64_t startAddress, const MemorySize & size);
        virtual ~MemoryUnit64();

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
        MemorySize              size() const { return _size; }
        void                    setSize(const MemorySize & size) { _size = size; }

        //////////
        //  Implementation
    private:
        //  Configuration
        uint64_t                _startAddress;
        MemorySize              _size;
    };

    //////////
    //  The standard "64-bit RAM unit" which resides entirely in host RAM
    class EMUONE_CORE_EXPORT ResidentRamUnit64 : public MemoryUnit64
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ResidentRamUnit64)

        friend class ResidentRamUnit64Editor;

        //////////
        //  Construction/destruction
    public:
        ResidentRamUnit64(const QString & name, uint64_t startAddress, const MemorySize & size);
        virtual ~ResidentRamUnit64();

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
    class EMUONE_CORE_EXPORT ResidentRomUnit64 : public MemoryUnit64
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ResidentRomUnit64)

        //////////
        //  Construction/destruction
    public:
        ResidentRomUnit64(const QString & name, uint64_t startAddress, const MemorySize & size, const QString & contentFileName);
        virtual ~ResidentRomUnit64();

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

        //  Configuration
        QString                 _contentFileName;

        //  Runtime state
        uint8_t *               _data = nullptr;
        size_t                  _dataSize = 0;
    };

    //////////
    //  The standard "64-bit memory bus" component
    class EMUONE_CORE_EXPORT MemoryBus64 : public Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MemoryBus64)

        //////////
        //  Construction/destruction
    public:
        MemoryBus64();
        virtual ~MemoryBus64();

        //////////
        //  Component
    public:
        virtual ComponentType *     type() const override;
        virtual ComponentEditor *   createEditor(QWidget * parent) override;
        virtual QString             shortStatus() const override;
        virtual ComponentUi *       createUi() override;

        //////////
        //  Component (state control) - all thread-safe
    public:
        virtual State       state() const override;
        virtual void        connect() override;
        virtual void        initialise() override;
        virtual void        start() override;
        virtual void        stop() noexcept override;
        virtual void        deinitialise() noexcept override;
        virtual void        disconnect() noexcept override;

        //////////
        //  Operations (serialisation)
    public:
        virtual void        serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void        deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  Implementation
    private:
        //  Runtime state
        size_t              _numMemoryUnits = 0;
        MemoryUnit64 *      _memoryUnits = nullptr; //  array of _numMemoryUnits, sorted by "startAddress"
    };
}

//  End of emuone-core/StandardMemory.hpp
