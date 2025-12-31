//
//  emuone-util/Sync.hpp - Synchronisation helpers
//
//////////

namespace emuone::util
{
    /// \class SynchronisationObject emuone-util/API.hpp
    /// \brief A generic object that can be used for thread synchronisation.
    class EMUONE_UTIL_PUBLIC SynchronisationObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SynchronisationObject)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        SynchronisationObject() = default;

        /// \brief
        ///     The class destructor.
        virtual ~SynchronisationObject() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     "Grabs" this synchronization object, idle-waiting
        ///     until it is available for grabbing.
        virtual void    grab() = 0;

        /// \brief
        ///     "Grabs" this synchronization object, waiying
        ///     until it is available for grabbing OR the specified
        ///     timeout expires.
        /// \param timeoutMs
        ///     The timeout, in milliseconds, to want for.
        /// \return
        ///     True on grab success, false on timeout.
        virtual bool    tryGrab(int timeoutMs) = 0;

        /// \brief
        ///     "Releases" a grabbed synchronization object.
        virtual void    release() = 0;
    };

    /// \class Mutex emuone-util/API.hpp
    /// \brief An extended mutex - only one thread can
    ///        have it "grabbed"/"locked" at a time.
    class EMUONE_UTIL_PUBLIC Mutex final
        :   public SynchronisationObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Mutex)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs an initially un-"grabbed" mutex.
        Mutex() = default;

        /// \brief
        ///     The class destructor.
        virtual ~Mutex() = default;

        //////////
        //  SynchronisationObject
    public:
        virtual void    grab() override { lock(); }
        virtual bool    tryGrab(int timeoutMs) override { return tryLock(timeoutMs); }
        virtual void    release() override { unlock(); }

        //////////
        //  Operations
    public:
        /// \brief
        ///     "Locks" this Mutex, idle-waiting until it is
        ///     available for locking.
        /// \details
        ///     Same as "grab()".
        ///     A thread is allowed to repeatedly "lock" the same
        ///     Mutex; later it must "unlock" that Mutex an equal
        ///     number of times before another thread can "lock" it.
        void            lock();

        /// \brief
        ///     "Locks" this Mutex, waiying until it is available for
        ///     locking OR the specified timeout expires.
        /// \details
        ///     Same as "tryGrab(timeoutMs)".
        ///     A thread is allowed to repeatedly "lock" the same
        ///     Mutex; later it must "unlock" that Mutex an equal
        ///     number of times before another thread can "lock" it.
        /// \param timeoutMs
        ///     The timeout, in milliseconds, to want for.
        /// \return
        ///     True on lock success, false on timeout.
        bool            tryLock(int timeoutMs);

        /// \brief
        ///     "Unlocks" a locked Mutex.
        /// \details
        ///     Same as "release()".
        void            unlock();

        /// \brief
        ///     Checks whether this Mutex is locked by
        ///     the specified thread.
        /// \param thread
        ///     The thread to check toe Mutex state for.
        /// \return
        ///     True if this Mutex is locked by the
        ///     specified thread, else false.
        bool            isLockedBy(QThread * thread);

        /// \brief
        ///     Checks whether this Mutex is locked by
        ///     the current thread.
        /// \return
        ///     True if this Mutex is locked by the
        ///     current thread, else false.
        bool            isLockedByCurrentThread();

        //////////
        //  Implementation
    private:
        QRecursiveMutex _impl = {};

        //  Extended mutex state
        QMutex          _extendedStateGuard = {};
        int             _lockCount = 0;
        QThread *       _lockingThread = nullptr;
    };

    /// \class Spinlock emuone-util/API.hpp
    /// \brief A fast non-reentrant spinlock.
    class EMUONE_UTIL_PUBLIC Spinlock final
        :   public SynchronisationObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Spinlock)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs an initially un-"grabbed" mutex.
        Spinlock() = default;

        /// \brief
        ///     The class destructor.
        virtual ~Spinlock() = default;

        //////////
        //  SynchronisationObject
    public:
        virtual void    grab() override
        {
            while (_flag.test_and_set(std::memory_order_acquire))
            {   //  Give other threads a chance
                std::this_thread::yield();
            }
        }

        virtual bool    tryGrab(int timeoutMs) override
        {   //  No timeout is a special case
            if (timeoutMs <= 0)
            {
                return !_flag.test_and_set(std::memory_order_acquire);
            }
            //  General case
            QElapsedTimer timer;
            timer.start();
            while (_flag.test_and_set(std::memory_order_acquire))
            {   //  Give other threads a chance
                std::this_thread::yield();
                if (timer.elapsed() > timeoutMs)
                {   //  Give up
                    return false;
                }
            }
            //  Success!
            return true;
        }

        virtual void    release() override
        {   //  Release lock: clear() sets the flag to false.
            _flag.clear(std::memory_order_release);
        }

        //////////
        //  Operations
    public:
        /// \brief
        ///     "Locks" this Spinlock, idle-waiting until it is
        ///     available for locking.
        /// \details
        ///     Same as "grab()".
        ///     A thread is NOT allowed to repeatedly "lock"
        ///     the same Spinlock.
        void            lock() { grab(); }

        /// \brief
        ///     "Locks" this Mutex, waiying until it is available for
        ///     locking OR the specified timeout expires.
        /// \details
        ///     Same as "tryGrab(timeoutMs)".
        ///     A thread is NOT allowed to repeatedly "lock"
        ///     the same Spinlock.
        /// \param timeoutMs
        ///     The timeout, in milliseconds, to want for.
        /// \return
        ///     True on lock success, false on timeout.
        bool            tryLock(int timeoutMs) { return tryGrab(timeoutMs); }

        /// \brief
        ///     "Unlocks" a locked Spinlock.
        /// \details
        ///     Same as "release()".
        void            unlock() { release(); }

        /// \brief
        ///     Checks whether this Mutex is locked by
        ///     the specified thread.
        /// \param thread
        ///     The thread to check toe Mutex state for.
        /// \return
        ///     True if this Mutex is locked by the
        ///     specified thread, else false.
        bool            isLockedBy(QThread * thread);

        /// \brief
        ///     Checks whether this Mutex is locked by
        ///     the current thread.
        /// \return
        ///     True if this Mutex is locked by the
        ///     current thread, else false.
        bool            isLockedByCurrentThread();

        //////////
        //  Implementation
    private:
        std::atomic_flag    _flag = ATOMIC_FLAG_INIT;
    };

    /// \class Lock emuone-util/API.hpp
    /// \brief
    ///     A helper object that "grabs" a synchronisation object
    ///     in constructor and "released" it in destructor.
    class EMUONE_UTIL_PUBLIC Lock final
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Lock)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor; "grabs" the synchronization object.
        /// \param guard
        ///     The synchronization object to "grab".
        explicit Lock(SynchronisationObject & guard)
            :   _guard(guard) { _guard.grab(); }

        /// \brief
        ///     The class destructor; "releases" the synchronization
        ///     object specified to the lock constructor.
        ~Lock() { _guard.release(); }

        //////////
        //  Implementation
    private:
        SynchronisationObject & _guard;
    };

    /// \class BlockingQueue emuone-util/API.hpp
    /// \brief A "blocking inter-thread queue" ADT.
    template <class T>
    class BlockingQueue
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BlockingQueue)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs an empty queue.
        BlockingQueue()
            :   _data(),
                _dataSize(),
                _dataGuard() {}

        /// \brief
        ///     The class destructor.
        ~BlockingQueue() {}

        //////////
        //  Operations
    public:
        /// \brief
        ///     Adds value to the end of the queue.
        /// \param value
        ///     The value to add.
        void        enqueue(const T & value);

        /// \brief
        ///     Removes the value at the start of the queue and returns
        ///     it, idle-waiting until one is actually available.
        /// \return
        ///     The value removed from the head of the queue.
        T           dequeue();

        /// \brief
        ///     Attempts to remove and store the item at the start
        ///     of the queue.
        /// \param value
        ///     The placeholder to store the value to if available.
        ///     Remains unchanged if the call times out.
        /// \param timeoutMs
        ///     The timeout, in milliseconds, to wait for the queue
        ///     to provide a value before giving up.
        /// \return
        ///     True on success (storing the value), false on timeout.
        bool        tryDequeue(T & value, int timeoutMs);

        //////////
        //  Implementaion
    private:
        QQueue<T>   _data;
        QSemaphore  _dataSize;  //  value == _data.size()
        Spinlock    _dataGuard;
    };

    template <class T>
    void BlockingQueue<T>::enqueue(const T & value)
    {
        Lock _(_dataGuard);
        _data.enqueue(value);
        _dataSize.release();
    }

    template <class T>
    T BlockingQueue<T>::dequeue()
    {
        _dataSize.acquire();
        Lock _(_dataGuard);
        return _data.dequeue();
    }

    template <class T>
    bool BlockingQueue<T>::tryDequeue(T & value, int timeoutMs)
    {
        if (_dataSize.tryAcquire(1, timeoutMs))
        {
            Lock _(_dataGuard);
            value = _data.dequeue();
            return true;
        }
        return false;
    }
}

//  End of emuone-util/Sync.hpp
