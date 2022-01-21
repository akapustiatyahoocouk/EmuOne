//
//  emuone-util/Sync.hpp
//
//  Synchronisation helpers
//
//////////

namespace util
{
    //////////
    //  An extended mutex
    class EMUONE_UTIL_EXPORT Mutex
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Mutex)

        //////////
        //  Construction/destruction
    public:
        Mutex();
        virtual ~Mutex();

        //////////
        //  Operations
    public:
        void                lock();
        bool                tryLock(int timeoutMs);
        void                unlock();

        bool                isLockedBy(QThread * thread);
        bool                isLockedByCurrentThread();

        //////////
        //  Implementation
    private:
        QRecursiveMutex     _impl = {};

        //  Extended mutex state
        QMutex              _extendedStateGuard = {};
        int                 _lockCount = 0;
        QThread *           _lockingThread = nullptr;
    };
}

//  End of emuone-util/Sync.hpp
