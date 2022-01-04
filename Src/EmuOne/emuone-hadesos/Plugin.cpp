//
//  emuone-hadesos/Plugin.cpp
//
//  The emuone-hadesos plugin
//
//////////
#include "emuone-hadesos/API.hpp"

namespace hadesos
{
    //////////
    //  The "emuone-hadesos" plugin
    class Plugin : public util::Plugin
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Plugin)

        //////////
        //  Construction/destruction
    public:
        Plugin() {}
        virtual ~Plugin() {}

        //////////
        //  Plugin
    public:
        virtual void initialise()
        {
            core::ComponentType::registerComponentType(HadesOs::Type::instance());
        }
    };
}

using namespace hadesos;
BEGIN_PLUGIN_TABLE
    PLUGIN_TABLE_ITEM(Plugin)
END_PLUGIN_TABLE

//  End of emuone-hadesos/Plugin.cpp
