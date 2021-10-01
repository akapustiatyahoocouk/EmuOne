//
//  emuone-core/Plugin.cpp
//
//  The emuone-core plugin
//
//////////
#include "emuone-core/API.hpp"

//////////
//  The "emuone-core" plugin
class CorePlugin : public Plugin
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CorePlugin)

    //////////
    //  Construction/destruction
public:
    CorePlugin() {}
    virtual ~CorePlugin() {}

    //////////
    //  Plugin
public:
    virtual void initialise()
    {
    }
};

BEGIN_PLUGIN_TABLE
    PLUGIN_TABLE_ITEM(CorePlugin)
END_PLUGIN_TABLE

//  End of emuone-core/Plugin.cpp
