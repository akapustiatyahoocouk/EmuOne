//
//  emuone-scp360/Objects.hpp
//
//  The SCP/360 kernel objects
//
//////////
#pragma once

namespace scp360
{
    //////////
    //  A generic "kernel object"
    class EMUONE_SCP360_EXPORT Object
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Object)

        //////////
        //  Types
    public:
        using Id = uint16_t;

        //////////
        //  Construction/destruction
    public:
        Object(Scp * scp, Id id);
        virtual ~Object();

        //////////
        //  Operations
    public:
        Scp *               scp() const { return _scp; }
        Id                  id() const { return _id; }

        //////////
        //  Implementation
    private:
        Scp *const          _scp;   //  ...managing this object
        const Id            _id;
    };

    //////////
    //  A “segment” is a continuous area of storage that can be accessed by a
    //  user program that has the right to do so.
    class EMUONE_SCP360_EXPORT Segment : public Object
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Segment)

        friend class ObjectManager;

        //////////
        //  Types
    public:
        //  Segment flags
        enum class Flags : uint16_t
        {
            Sticky = 0x0001,        //  Normally, a segment is destroyed when its mapping and lock
                                    //  counts both reach 0. However, if the segment is marked
                                    //   “sticky” it remains in existence.
            //   Flag combinations
            None = 0x0000,          //  None of the flags
            All = 0x0001            //  All of the flags
        };

        //  The location of the segment's content
        class EMUONE_SCP360_EXPORT ContentLocation
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ContentLocation)

            //////////
            //  Construction/destruction
        public:
            ContentLocation() {}
            virtual ~ContentLocation() { Q_ASSERT(_canDelete); }

            //////////
            //  Implementation
        private:
            bool            _canDelete = false;
        };

        //  The segment's content is in main storage
        class EMUONE_SCP360_EXPORT StorageContentLocation : public ContentLocation
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(StorageContentLocation)

            //////////
            //  Construction/destruction
        public:
            explicit StorageContentLocation(uint32_t address)
              :     _address(address)
            {
                Q_ASSERT((_address & 0x00FFFFFF) == _address);
                Q_ASSERT((_address & 0x000007FF) == 0);
            }
            virtual ~StorageContentLocation() {}

            //////////
            //  Operations
        public:
            uint32_t        address() const { return _address; }

            //////////
            //  Implementation
        private:
            uint32_t        _address;   //  ...where segment content starts; always at 2K boundary
        };

        //////////
        //  Construction/destruction - from ObjectManager only
    private:
        Segment(Scp * scp, Id id,
                const QString & name, uint32_t size, Flags flags, uint32_t address);
        virtual ~Segment();

        //////////
        //  Operations
    public:
        static bool         isValidName(const QString & name);

        QString             name() const { return _name; }
        uint32_t            size() const { return _size; }
        Flags               flags() const { return _flags; }
        SegmentMappingList  mappings() const { return _mappings; }
        SegmentLockList     locks() const { return _locks; }
        ContentLocation *   contentLocation() const { return _contentLocation; }
        uint16_t            holdCount() const { return _holdCount; }

        //////////
        //  Implementation
    private:
        const QString       _name;
        uint32_t            _size;
        Flags               _flags;
        SegmentMappingList  _mappings;  //  all mappings for this segment
        SegmentLockList     _locks;     //  all locks for this segment
        ContentLocation *   _contentLocation;
        uint16_t            _holdCount;
    };

    EMUONE_SCP360_EXPORT Segment::Flags operator & (Segment::Flags op1, Segment::Flags op2);
    EMUONE_SCP360_EXPORT Segment::Flags operator | (Segment::Flags op1, Segment::Flags op2);
    EMUONE_SCP360_EXPORT Segment::Flags & operator &= (Segment::Flags & op1, Segment::Flags op2);
    EMUONE_SCP360_EXPORT Segment::Flags & operator |= (Segment::Flags & op1, Segment::Flags op2);

    //////////
    //  Describes a "mapping" of a specific Segment into a particular Process' address space
    class EMUONE_SCP360_EXPORT SegmentMapping final : public Object
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SegmentMapping)

        //////////
        //  Construction/destruction
    public:
        SegmentMapping(Scp * scp, Id id,
                       Segment * segment, Process * process);
        virtual ~SegmentMapping();

        //////////
        //  Operations
    public:
        Segment *           segment() const { return _segment; }
        Process *           process() const { return _process; }
        uint16_t            count() const { return _count; }

        //////////
        //  Implementation
    private:
        Segment *const      _segment;
        Process *const      _process;
        uint16_t            _count;
    };

    //////////
    //  Describes a "lock" of a specific Segment by a particular Process
    class EMUONE_SCP360_EXPORT SegmentLock final : public Object
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SegmentLock)

        //////////
        //  Construction/destruction
    public:
        SegmentLock(Scp * scp, Id id,
                    Segment * segment, Process * process);
        virtual ~SegmentLock();

        //////////
        //  Operations
    public:
        Segment *           segment() const { return _segment; }
        Process *           process() const { return _process; }
        uint16_t            count() const { return _count; }

        //////////
        //  Implementation
    private:
        Segment *const      _segment;
        Process *const      _process;
        uint16_t            _count;
    };

    //////////
    //  A SCP/360 application Process
    class EMUONE_SCP360_EXPORT Process : public Object
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Process)

        friend class EmulatedProcess;
        friend class ObjectManager;

        //////////
        //  Constants
    public:
        static const uint16_t   MinStaticPriority = 0;
        static const uint16_t   MaxStaticPriority = 31;
        static const uint16_t   DefaultStaticPriority = 7;
        static const int16_t    MinDynamicPriority = -3;
        static const int16_t    MaxDynamicPriority = 3;

        //////////
        //  Types
    public:
        enum class Flags : uint16_t
        {
            System = 0x01,  //  A “system” process runs with the protection key 0 in PSW
            //   Flag combinations
            None = 0x00,    //  None of the flags
            All = 0x01      //  All of the flags
        };

        enum class State : uint16_t
        {
            Prepared,
            Ready,
            Active,
            Suspended,
            Waiting,
            SuspendedWaiting,
            Finished
        };

        //////////
        //  Construction/destruction
    public:
        Process(Scp * scp, Id id,
                const QString & name, Flags flags, Process * parent);
        virtual ~Process();

        //////////
        //  Operations
    public:
        static bool             isValidName(const QString & name);

        QString                 name() const { return _name; }
        Flags                   flags() const { return _flags; }
        State                   state() const { return _state; }
        void                    setState(State state);

        uint16_t                suspendCount() const { return _suspendCount; }

        ErrorCode               setEnvironmentVariable(const QString & name, const QString & scalarValue);
        ErrorCode               setEnvironmentVariable(const QString & name, const QStringList & listValue);

        //////////
        //  Implementation
    private:
        QString                 _name;
        Flags                   _flags;
        State                   _state = State::Prepared;

        Process *               _parent;    //  nullptr == none
        ProcessList             _children;

        QMap<QString, EnvironmentVariable*> _environment;   //  name -> variable

        uint16_t                _suspendCount = 0;
        uint16_t                _staticPriority;
        int16_t                 _dynamicPriority;
        ErrorCode               _exitCode = ErrorCode::ERR_UNK;
        uint16_t                _holdCount = 0;
    };

    EMUONE_SCP360_EXPORT Process::Flags operator & (Process::Flags op1, Process::Flags op2);
    EMUONE_SCP360_EXPORT Process::Flags operator | (Process::Flags op1, Process::Flags op2);
    EMUONE_SCP360_EXPORT Process::Flags & operator &= (Process::Flags & op1, Process::Flags op2);
    EMUONE_SCP360_EXPORT Process::Flags & operator |= (Process::Flags & op1, Process::Flags op2);

    //////////
    //  A process environment variable
    class EMUONE_SCP360_EXPORT EnvironmentVariable final
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(EnvironmentVariable)

        //////////
        //  Construction/destruction
    public:
        EnvironmentVariable(const QString & name, const QString & scalarValue);
        EnvironmentVariable(const QString & name, const QStringList & listValue);

        //////////
        //  Operations
    public:
        static bool             isValidName(const QString & name);
        static bool             isValidScalarValue(const QString & scalarValue);
        static bool             isValidListValue(const QStringList & listValue);

        QString                 name() const { return _name; }
        QStringList             values() const { return _values; }
        int                     valueCount() const { return _values.size(); }
        QString                 valueAt(int index) const { return _values[index]; }

        //  Returns the value of this environment variable as a scalar
        QString                 toString() const;

        //  Returns a deep clone of this environment variable
        EnvironmentVariable *   clone() const;

        //////////
        //  Implementation
    private:
        QString                 _name;
        QStringList             _values;    //  never empty!
    };

    //////////
    //  A SCP/360 application Process that runs C++ code
    class EMUONE_SCP360_EXPORT EmulatedProcess : public Process
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(EmulatedProcess)

        friend class Scp;

        //////////
        //  Construction/destruction - sub-classes only
    protected:
        EmulatedProcess(Scp * scp, uint16_t id, const QString & name, Flags flags, Process * parent);
        virtual ~EmulatedProcess();

        //////////
        //  Operations
    public:
        virtual EmulatedApplication *   application() const = 0;
        void                    start();
        void                    stop(ErrorCode exitCode);
    protected:
        virtual ErrorCode       run() = 0;

        //////////
        //  System calls for an emulated process (all synchronous!)
    public:
        class EMUONE_SCP360_EXPORT SystemCalls final
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SystemCalls)

            friend class EmulatedProcess;

            //////////
            //  Construction/destruction - from friends only
        private:
            explicit SystemCalls(EmulatedProcess * emulatedProcess) : _emulatedProcess(emulatedProcess) {}
            ~SystemCalls() {}

            //////////
            //  Operations
        public:
            //  Makes the specified "systemCall", returning only after the
            //  system call has completed and its outcome is known.
            //  The caller (who is the one that created the systemCall) is
            //  then responsible for deleting the "systemCall".
            ErrorCode               makeSystemCall(SystemCall * systemCall);

            //  Writes a single message (record) to the "operator console".
            //  If the "text" is too long, it is truncated at the right edge to fit to
            //  a single record (e.g. a single 80-column line on IBM 2741, etc.)
            //  Returns when the writing has completed one way or another.
            ErrorCode               writeToOperator(const QString & text);

    /*
    7.2.2	SC#EVCNT – Count Environment Variable Values
    This system call allows the process to determine the number of values that a specified variable in its environment has. As a special case, it can also be used to determine if the specified environment variable exists at all.

    Call gateway:	SVC   10
    Upon entry:	•	R1 = SC#EVCNT (X’00020101’)
    •	R2 = the address of the environment variable name.
    •	R3 = the length of the environment variable name.
    Upon return:	•	R15 = call status, ERR#OK for success, other ERR#... error codes for failure. Typically ERR#NOF, which means that the specified environment variable does not exist.
    •	R0 = the number of values the specified environment variable has; always 1 (scalar) or more upon success; undefined upon failure.
    Comments	•	Any tailing whitespace in the environment variable name is ignored.
    •	Any lower-case letters in the environment variable name are treated as upper-case.
    7.2.3	SC#EVGTL – Get Environment Variable Value (List)
    This system call allows the process to retrieve an individual value from the list of values associated with a given variable in its environment.

    Call gateway:	SVC   10
    Upon entry:	•	R1 = SC#EVGTL (X’00020102’)
    •	R2 = the address of the environment variable name.
    •	R3 = the length of the environment variable name.
    •	R4 = the 0-based index of the value to retrieve.
    •	R5 = the address of the buffer to store the value to.
    •	R6 = the length of the buffer to store the value to.
    Upon return:	•	R15 = call status, ERR#OK for success, other ERR#... error codes for failure.
    •	R0 = the number of characters actually stored into the supplied buffer (upon success); undefined (upon failure).
    •	Other registers are unchanged.
    Comment	•	Any tailing whitespace in the environment variable name is ignored.
    •	Any lower-case letters in the environment variable name are treated as upper-case.
    •	If the environment variable in question does not exist, or the index exceeds the number of values it has, R15 will be ERR#NOF upon return.
    •	If the specified buffer is too small to hold the entire value, only the initial portion of the value (as much as fits into the buffer) will be stored and R15 will be ERR#BTS upon return.
    •	If the specified buffer is larger than the value, only the initial portion of the buffer is filled, and the rest of the buffer is not affected; the number of characters actually stored is returned in R0.
    7.2.4	SC#EVGTS – Get Environment Variable Value (Scalar)
    This system call allows the process to retrieve the complete value associated with a given variable in its environment.

    Call gateway:	SVC   10
    Upon entry:	•	R1 = SC#EVGTS
    •	R2 = the address of the environment variable name.
    •	R3 = the length of the environment variable name.
    •	R4 = the address of the buffer to store the value to.
    •	R5 = the length of the buffer to store the value to.
    Upon return:	•	R15 = call status, ERR#OK for success, other ERR#... error codes for failure.
    •	R0 = the number of characters actually stored into the supplied buffer (upon success); undefined (upon failure).
    •	Other registers are unchanged.
    Comment	•	Any tailing whitespace in the environment variable name is ignored.
    •	Any lower-case letters in the environment variable name are treated as upper-case.
    •	If the environment variable in question does not exist, or the index exceeds the number of values it has, R15 will be ERR#NOF upon return.
    •	If the environment variable has a list of more than 1 values, the string stored into the buffer will contain all these values separated by commas and enclosed in parentheses, e.g. (VAL1,VAL2,VAL3). The system call will fail unless the buffer is large enough to hold the entire list.
    •	If the specified buffer is too small to hold the entire value, only the initial portion of the value (as much as fits into the buffer) will be stored and R15 will be ERR#BTS upon return.
    •	If the specified buffer is larger than the value, only the initial portion of the buffer is filled, and the rest of the buffer is not affected; the number of characters actually stored is returned in R0.



    SC#EVSTL EQU   X'00020104'  SET ENVIRONMENT VARIABLE VALUE (LIST)
    */
            ErrorCode               setEnvironmentVariableValue(const QString & name, const QString & scalarValue);
            ErrorCode               setEnvironmentVariableValue(Process * targetProcess, const QString & name, const QString & scalarValue);
    /*
    SC#EVADL EQU   X'00020106'  ADD TO ENVIRONMENT VARIABLE VALUE (LIST)
    SC#EVDEL EQU   X'00020107'  DELETE ENVIRONMENT VARIABLE
    */
            //////////
            //  Implementation
        private:
            EmulatedProcess *const  _emulatedProcess;
        };
        SystemCalls             systemCalls;

        //////////
        //  Operations
    public:
        void                    markSystemCallComplete();

        //////////
        //  Implementation
    private:
        QSemaphore              _systemCallCompletionSemaphore;

        //////////
        //  Threads
    private:
        //  Each EmulatedProcess runs on a separate worker thread
        class _WorkerThread : public QThread
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_WorkerThread)

            //////////
            //  Construction/destruction
        public:
            explicit _WorkerThread(EmulatedProcess * emulatedProcess);
            virtual ~_WorkerThread();

            //////////
            //  QThread
        public:
            virtual void    run() override;

            //////////
            //  Implementation
        private:
            EmulatedProcess *const  _emulatedProcess;
        };
        _WorkerThread *     _workerThread = nullptr;
    };

    //////////
    //  A generic "device" available to SCP/360
    class EMUONE_SCP360_EXPORT Device : public Object
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Device)

        //////////
        //  Types
    public:
        enum class Flags
        {
            WriteBlock = 0x00000001,        //  DeviceDriver supports writeBlock() call
            //  Flag combinations
            None = 0x00000000,  //  None of the flags
            All = 0x00000003    //  All of the flags
        };

        enum class State
        {
            Unknown,            //  device state is unknown
            Ready,              //  device is ready for next I/O
            Busy,               //  device is busy performing I/O
            NonOperational      //  device is not operational (switched off, etc.)
        };

        //////////
        //  Construction/destruction
    public:
        Device(Scp * scp, Id id, const QString & name, Flags Flags, DeviceDriver * driver);
        virtual ~Device();

        //////////
        //  Operations
    public:
        QString             name() const { return _name; }
        Flags               flags() const { return _flags; }
        State               state() const { return _state; }
        void                setState(State state) { _state = state; }
        DeviceDriver *      driver() const { return _driver; }

        //////////
        //  Implementation
    private:
        const QString       _name;
        const Flags         _flags;
        volatile State      _state = State::Unknown;    //  set asynchronously by device driver
        DeviceDriver *const _driver;
    };

    EMUONE_SCP360_EXPORT Device::Flags operator & (Device::Flags op1, Device::Flags op2);
    EMUONE_SCP360_EXPORT Device::Flags operator | (Device::Flags op1, Device::Flags op2);
    EMUONE_SCP360_EXPORT Device::Flags & operator &= (Device::Flags & op1, Device::Flags op2);
    EMUONE_SCP360_EXPORT Device::Flags & operator |= (Device::Flags & op1, Device::Flags op2);

    //////////
    //  A generic "pysical device" available to SCP/360
    class EMUONE_SCP360_EXPORT PhysicalDevice : public Device
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PhysicalDevice)

        //////////
        //  Construction/destruction
    public:
        PhysicalDevice(Scp * scp, Id id, ibm360::Device * hardwareDevice, Flags flags, DeviceDriver * driver);
        virtual ~PhysicalDevice();

        //////////
        //  Operations
    public:
        static bool         isValidName(const QString & name);

        //////////
        //  Operations
    public:
        ibm360::Device *    hardwareDevice() const { return _hardwareDevice; }

        //////////
        //  Implementation
    private:
        ibm360::Device *    _hardwareDevice;    //  ...represented by this PhysicalDevice in SCP/360

        //  Helpers
        static QString      _nameFromAddress(uint16_t address);
    };

    //////////
    //  A generic "logical device" available to SCP/360
    class EMUONE_SCP360_EXPORT LogicalDevice : public Device
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(LogicalDevice)

        //////////
        //  Construction/destruction
    public:
        LogicalDevice(Scp * scp, Id id, const QString & name, Flags flags, DeviceDriver * driver);
        virtual ~LogicalDevice();

        //////////
        //  Operations
    public:
        static bool         isValidName(const QString & name);
    };
}

//  End of emuone-scp360/Objects.hpp
