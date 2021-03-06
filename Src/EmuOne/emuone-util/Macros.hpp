//
//  emuone-util/Macros.hpp
//
//  Helper macros
//
//////////

#define CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Clazz)  \
    private:                                    \
        Clazz(const Clazz &) = delete;          \
        void operator = (const Clazz &) = delete;

#define UTILITY_CLASS(Clazz)                    \
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Clazz)      \
    private:                                    \
        Clazz() = delete;                       \
        ~Clazz() = delete;

#define DECLARE_SINGLETON(Clazz)                \
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Clazz)      \
    private:                                    \
        Clazz();                                \
        ~Clazz();                               \
    public:                                     \
        static Clazz * instance();

#define IMPLEMENT_SINGLETON(Clazz)              \
    Clazz * Clazz::instance()                   \
    {                                           \
        static Clazz theInstance;               \
        return &theInstance;                    \
    }

//  End of emuone-util/Macros.hpp
