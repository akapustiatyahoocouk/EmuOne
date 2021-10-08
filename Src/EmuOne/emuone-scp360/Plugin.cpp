//
//  emuone-scp360/Plugin.cpp
//
//  The emuone-scp360 plugin
//
//////////
#include "emuone-scp360/API.hpp"

namespace scp360
{
    //////////
    //  The "emuone-scp360" plugin
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
        }
    };
}

using namespace scp360;
BEGIN_PLUGIN_TABLE
    PLUGIN_TABLE_ITEM(Plugin)
END_PLUGIN_TABLE

//  End of emuone-scp360/Plugin.cpp
