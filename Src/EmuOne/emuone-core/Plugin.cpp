//
//  emuone-core/Plugin.cpp
//
//  The emuone-core plugin
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  The "emuone-core" plugin
class CorePlugin : public util::Plugin
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
        ComponentCategory::registerComponentCategory(StandardComponentCategories::Memory::getInstance());
        ComponentCategory::registerComponentCategory(StandardComponentCategories::Processors::getInstance());
        ComponentCategory::registerComponentCategory(StandardComponentCategories::Terminals::getInstance());
        ComponentCategory::registerComponentCategory(StandardComponentCategories::Software::getInstance());
    }
};

BEGIN_PLUGIN_TABLE
    PLUGIN_TABLE_ITEM(CorePlugin)
END_PLUGIN_TABLE

//  End of emuone-core/Plugin.cpp
