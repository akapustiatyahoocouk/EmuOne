//
//  emuone-util/PluginManager.hpp
//
//  Plugin framework
//
//////////

namespace util
{
    //////////
    //  A generic "plugin" is an agent doscovered dynamically at start-up and initialised
    class EMUONE_UTIL_EXPORT Plugin
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Plugin)

        //////////
        //  Construction/destruction
    public:
        Plugin() {}
        virtual ~Plugin() {}

        //////////
        //  Operations
    public:
        //  Initialises this plugin. Returns on success, throws on failure.
        virtual void            initialise() = 0;
    };
    using PluginList = QList<Plugin*>;

    //////////
    //  The plugin discoverer / manager
    class EMUONE_UTIL_EXPORT PluginManager
    {
        UTILITY_CLASS(PluginManager)

        //////////
        //  Operations
    public:
        static void             loadPlugins();

        //////////
        //  Implementation
    private:
        static QSet<QString>    _processedDllPaths;
        static PluginList       _loadedPlugins;
        static PluginList       _initialisezPlugins;

        typedef void (*PluginExportProc)(PluginList & pluginList);
    };
}

//////////
//  Helper macros for plugin export
#define BEGIN_PLUGIN_TABLE                                  \
    extern "C" Q_DECL_EXPORT                                \
    void PluginExportProc(util::PluginList & pluginList)    \
    {                                                       \
        pluginList.clear();

#define PLUGIN_TABLE_ITEM(Clazz)                            \
        static Clazz the##Clazz;                            \
        pluginList.append(&the##Clazz);

#define END_PLUGIN_TABLE                                    \
    }

//  End of emuone-util/PluginManager.hpp
