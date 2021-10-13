//
//  emuone-scp360/Process.hpp
//
//  A SCP/360 Process
//
//////////
#pragma once

namespace scp360
{
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
    //  A SCP/360 application Process
    class EMUONE_SCP360_EXPORT Process
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Process)

        friend class EmulatedProcess;

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
            System = 0x01,
            //   Flag combinations
            None = 0x00,
            All = 0x01
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
        Process(Scp * scp, uint16_t id, const QString & name, Flags flags, Process * parent);
        virtual ~Process();

        //////////
        //  Operations
    private:
        Scp *                   scp() const { return _scp; }
        uint16_t                id() const { return _id; }

        //////////
        //  Implementation
    private:
        Scp *                   _scp;   //  ...which runs this process
        uint16_t                _id;
        QString                 _name;
        Flags                   _flags;
        State                   _state = State::Prepared;

        Process *               _parent;    //  nullptr == none
        ProcessList             _children;

        QMap<QString, EnvironmentVariable*> _environment;   //  name -> variable

        uint16_t                _suspendCount = 0;
        uint16_t                _staticPriority;
        int16_t                 _dynamicPriority;
        uint16_t                _exitCode = 0;
        uint16_t                _holdCount = 0;
    };

    //////////
    //  A SCP/360 application Process that runs C++ code3
    class EMUONE_SCP360_EXPORT EmulatedProcess : public Process
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(EmulatedProcess)

        //////////
        //  Construction/destruction
    public:
        EmulatedProcess(Scp * scp, uint16_t id, const QString & name, Flags flags, Process * parent,
                        EmulatedApplication * emulatedApplication);
        virtual ~EmulatedProcess();

        //////////
        //  Operations
    public:
        void                    start();
    protected:
        virtual uint16_t        run() = 0;

        //////////
        //  Implementation
    private:
        EmulatedApplication *   _emulatedApplication;

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
            //  Operations
        public:
            void            requestStop() { _stopRequested = true;  }

            //////////
            //  Implementation
        private:
            EmulatedProcess *const  _emulatedProcess;
            volatile bool   _stopRequested = false;
        };
        _WorkerThread *     _workerThread = nullptr;
    };
}

//  End of emuone-scp360/Process.hpp
