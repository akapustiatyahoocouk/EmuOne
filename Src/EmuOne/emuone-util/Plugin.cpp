//
//  emuone-util/Plugin.cpp
//
//  The emuone-util plugin
//
//////////
#include "emuone-util/API.hpp"
using namespace util;

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
        CharacterSet::registerCharacterSet(Iso88591CharacterSet::instance());
        CharacterSet::registerCharacterSet(Cp037CharacterSet::instance());

        MessageDigest::registerMessageDigest(Sha1MessageDigest::instance());
    }
};

BEGIN_PLUGIN_TABLE
    PLUGIN_TABLE_ITEM(UtilPlugin)
END_PLUGIN_TABLE

//  End of emuone-util/Plugin.cpp
