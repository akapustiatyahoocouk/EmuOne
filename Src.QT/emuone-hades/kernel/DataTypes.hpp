//
//  emuone-hades/kernel/DataTypes.hpp - Kernel data types
//
//  EmuOne
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////

namespace emuone::hades::kernel
{
    /// \brief The Kernel error codes.
    enum KErrno : uint16_t   //  not "enum class" - we want errno-like syntax
    {
        K_EOK = 0,          //  success
        K_E2BIG,            //  argument list too long
        K_EACCES,           //  permission denied
        K_EADDRINUSE,       //  address in use
        K_EADDRNOTAVAIL,    //  address not available
        K_EAFNOSUPPORT,     //  address family not supported
        K_EAGAIN,           //  resource unavailable try again
        K_EALREADY,         //  connection already in progress
        K_EBADF,            //  bad file descriptor
        K_EBADMSG,          //  bad message
        K_EBUSY,            //  device or resource busy
        K_ECANCELED,        //  operation canceled
        K_ECHILD,           //  no child process
        K_ECONNABORTED,     //  connection aborted
        K_ECONNREFUSED,     //  connection refused
        K_ECONNRESET,       //  connection reset
        K_EDEADLK,          //  resource deadlock would occur
        K_EDESTADDRREQ,     //  destination address required
        K_EDOM,             //  argument out of domain
        K_EEXIST,           //  file exists
        K_EFAULT,           //  bad address
        K_EFBIG,            //  file too large
        K_EHOSTUNREACH,     //  host unreachable
        K_EIDRM,            //  identifier removed
        K_EILSEQ,           //  illegal byte sequence
        K_EINPROGRESS,      //  operation in progress
        K_EINTR,            //  interrupted
        K_EINVAL,           //  invalid argument
        K_EIO,              //  io error
        K_EISCONN,          //  already connected
        K_EISDIR,           //  is a directory
        K_ELOOP,            //  too many symbolic link levels
        K_EMFILE,           //  too many files open
        K_EMLINK,           //  too many links
        K_EMSGSIZE,         //  message size
        K_ENAMETOOLONG,     //  filename too long
        K_ENETDOWN,         //  network down
        K_ENETRESET,        //  network reset
        K_ENETUNREACH,      //  network unreachable
        K_ENFILE,           //  too many files open in system
        K_ENOBUFS,          //  no buffer space
        K_ENODATA,          //  no message available
        K_ENODEV,           //  no such device
        K_ENOENT,           //  no such file or directory
        K_ENOEXEC,          //  executable format error
        K_ENOLCK,           //  no lock available
        K_ENOLINK,          //  no link
        K_ENOMEM,           //  not enough memory
        K_ENOMSG,           //  no message
        K_ENOPROTOOPT,      //  no protocol option
        K_ENOSPC,           //  no space on device
        K_ENOSR,            //  no stream resources
        K_ENOSTR,           //  not a stream
        K_ENOSYS,           //  function not supported
        K_ENOTCONN,         //  not connected
        K_ENOTDIR,          //  not a directory
        K_ENOTEMPTY,        //  directory not empty
        K_ENOTRECOVERABLE,  //  state not recoverable
        K_ENOTSOCK,         //  not a socket
        K_ENOTSUP,          //  not supported
        K_ENOTTY,           //  inappropriate io control operation
        K_ENXIO,            //  no such device or address
        K_EOPNOTSUPP,       //  operation not supported
        K_EOVERFLOW,        //  value too large
        K_EOWNERDEAD,       //  owner dead
        K_EPERM,            //  operation not permitted
        K_EPIPE,            //  broken pipe
        K_EPROTO,           //  protocol error
        K_EPROTONOSUPPORT,  //  protocol not supported
        K_EPROTOTYPE,       //  wrong protocol type
        K_ERANGE,           //  result out of range
        K_EROFS,            //  read only file system
        K_ESPIPE,           //  invalid seek
        K_ESRCH,            //  no such process
        K_ETIME,            //  stream timeout
        K_ETIMEDOUT,        //  timed out
        K_ETXTBSY,          //  text file busy
        K_EWOULDBLOCK,      //  operation would block
        K_EXDEV,            //  cross device link
        K_EOTHER = 0xFFFF   //  other
    };

    enum KSignal
    {
        K_SIGHUP    = 1,    //  Hang up detected on controlling terminal or death of controlling process.
        K_SIGINT    = 2,    //  Interrupt signal (commonly initiated by Ctrl+C).
        K_SIGQUIT   = 3,    //  Quit signal (commonly initiated by Ctrl+\).
        K_SIGILL    = 4,    //  Illegal instruction.
        K_SIGTRAP   = 5,    //  Trace/breakpoint trap
        K_SIGABRT   = 6,    //  Abnormal termination (raised by the abort() system call).
        K_SIGBUS    = 7,    //  Bus error
        K_SIGFPE    = 8,    //  Floating-point exception (e.g., division by zero).
        K_SIGKILL   = 9,    //  Forcefully terminate a process. This signal cannot be caught, ignored, or handled.
        K_SIGUSR1   = 10,   //  User-defined signal 1.
        K_SIGSEGV   = 11,   //  Invalid memory reference (segmentation fault).
        K_SIGUSR2   = 12,   //  User-defined signal 2.
        K_SIGPIPE   = 13,   //  Write to a pipe that has no process reading from the other end.
        K_SIGALRM   = 14,   //  Alarm clock signal (used for timers).
        K_SIGTERM   = 15,   //  Software termination signal (sent by the kill command by default; can be caught and handled for clean shutdown).
        K_SIGCHLD   = 17,   //  Child process terminated or stopped.
        K_SIGCONT   = 18,   //  Continue executing if stopped.
        K_SIGSTOP   = 19,   //  Stop process execution. Cannot be caught or ignored.
        K_SIGTSTP   = 20,   //  Stop process initiated from terminal (commonly Ctrl+Z; can be handled or ignored).
        K_SIGBREAK  = 21,   //  Ctrl-Break sequence
        K_NSIG      = 22    //  maximum signal number + 1
    };
    using K_sigset_t = uint32_t;
    inline K_sigset_t K_sigmask(int signal)
    {
        return K_sigset_t(1) << signal;
    }

    /// \class Id emuone-hades/API.hpp
    /// \brief A generic ID of something within ght kernel.
    template <class T>
    class Id
    {
        static_assert(std::is_unsigned<T>::value, "T must be an unsigned type");
        static_assert(std::is_integral <T>::value, "T must be an integer type");

        friend class Kernel;

        //////////
        //  Constants
    private:
        static const T  _InvalidImpl = ~T(0);

        //////////
        //  Construction/destruction/assignment
    public:
        explicit constexpr Id(T impl) : _impl(impl) {}
    public:
        /// \brief
        ///     Constructs an invalid OID.
        Id() : _impl(_InvalidImpl) {}

        //  Default copy constructor, destructor and
        //  assignment are all OK.

        //////////
        //  Operators
    public:
        bool            operator == (const Id<T> & op2) const { return _impl == op2._impl; }
        bool            operator != (const Id<T> & op2) const { return _impl != op2._impl; }
        bool            operator <  (const Id<T> & op2) const { return _impl <  op2._impl; }
        bool            operator <= (const Id<T> & op2) const { return _impl <= op2._impl; }
        bool            operator >  (const Id<T> & op2) const { return _impl >  op2._impl; }
        bool            operator >= (const Id<T> & op2) const { return _impl >= op2._impl; }
        explicit        operator T() const { return _impl; }

        //////////
        //  Operations
    public:
        /// \brief
        ///     Checks whether this OID is valid.
        /// \return
        ///     True if this OID is valid (can be used by so e Object),
        ///     false if not.
        bool            isValid() const { return _impl != _InvalidImpl; }

        //////////
        //  Implementation
    private:
        T               _impl;
    };

    /// \brief The Kernel Object ID - unique.
    class EMUONE_HADES_PUBLIC Oid final : public Id<uint32_t>
    {
        //////////
        //  Constants
    public:
        //  Fixed OID range
        static const Oid    MinFixedOid;    //  inclusive
        static const Oid    MaxFixedOid;    //  inclusive

        //  Random OIDs
        static const Oid    MinRandomOid;   //  inclusive
        static const Oid    MaxRandomOid;   //  inclusive

        //////////
        //  Construction/destruction
    public:
        Oid() = default;
        explicit Oid(uint32_t impl) : Id<uint32_t>(impl) {}
    };

    /// \brief An ID of the Device, unique per Kernel.
    class EMUONE_HADES_PUBLIC DeviceTypeId final : public Id<uint16_t>
    {
        //////////
        //  Constants
    public:
        static const DeviceTypeId   Amd64Processor;
        static const DeviceTypeId   Amd64ProcessorCore;
        static const DeviceTypeId   HostProcessor;
        static const DeviceTypeId   HostProcessorCore;

        //////////
        //  Construction/destruction
    public:
        DeviceTypeId() = default;
        explicit DeviceTypeId(uint16_t impl) : Id<uint16_t>(impl) {}
    };

    /// \brief An ID of the Device, unique per DeviceType.
    using DeviceId = Id<uint16_t>;

    /// \brief //  An ID of a Processor, unique per Kernel.
    using ProcessorId = Id<uint8_t>;

    /// \brief An ID of a ProcessorCore, unique per Processor.
    using CoreId = Id<uint8_t>;

    ///\brief The Process/Thread priority class
    enum class PriorityClass
    {
        Idle,           ///< Only run where nothing else is ready.
        LowPriority,    ///< Low-priority round-robin, boost priority when passed over.
        Normal,         ///< Standard round-robin, boost priority when passed over..
        HighPriority,   ///< High-priority round-robin.
        TimeCritical,   ///< Oly preempt by other TimeCriticals.
        Realtime        ///< Never preempt until it yields.
    };

    typedef void (NativeThreadRunner::*NativeSignalHandler)(int);
    enum SignalDisposition  //  Not "enum class" for better syntax
    {
        K_SIG_TRM,  ///< Terminate process.
        K_SIG_IGN,  ///< Ignore the signal
        K_SIG_TRD,  ///< Terminate process, dump core.
        K_SIG_STP,  ///< Stop process.
        K_SIG_CNT,  ///< Continue process if stopped.
        //  Special values for signal() function
        K_SIG_DFL,  ///< Revert to default.
        K_SIG_UDF   ///< User-defined function.
    };
}

//  End of emuone-hades/kernel/DataTypes.hpp
