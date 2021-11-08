//
//  emuone-cereon/Plugin.cpp
//
//  The emuone-cereon plugin
//
//////////
#include "emuone-cereon/API.hpp"

namespace cereon
{
    //////////
    //  The "emuone-cereon" plugin
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
            core::Architecture::registerArchitecture(CereonWorkstationArchitecture::instance());
            CereonWorkstationArchitecture::instance()->addVirtualMachineTemplate(CereonWorkstationMiniTemplate::instance());

            core::ComponentType::registerComponentType(Processor1P1B::Type::instance());

            core::ComponentType::registerComponentType(ResidentRamUnit::Type::instance());
            core::ComponentType::registerComponentType(ResidentRomUnit::Type::instance());

        }
    };
}

using namespace cereon;
BEGIN_PLUGIN_TABLE
    PLUGIN_TABLE_ITEM(Plugin)
END_PLUGIN_TABLE

//  End of emuone-cereon/Plugin.cpp
