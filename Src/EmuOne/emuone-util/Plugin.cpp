//
//  emuone-util/Plugin.cpp
//
//  The emuone-util plugin
//
//////////
#include "emuone-util/API.hpp"

//////////
//  The "emuone-util" plugin
class UtilPlugin : public Plugin
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(UtilPlugin)

    //////////
    //  Construction/destruction
public:
    UtilPlugin() {}
    virtual ~UtilPlugin() {}

    //////////
    //  Plugin
public:
    virtual void initialise()
    {
    }
};

BEGIN_PLUGIN_TABLE
    PLUGIN_TABLE_ITEM(UtilPlugin)
END_PLUGIN_TABLE

//  End of emuone-util/Plugin.cpp
