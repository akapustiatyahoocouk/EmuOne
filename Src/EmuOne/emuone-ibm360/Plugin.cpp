//
//  emuone-ibm360/Plugin.cpp
//
//  The emuone-ibm360 plugin
//
//////////
#include "emuone-ibm360/API.hpp"

namespace ibm360
{
    //////////
    //  The "emuone-ibm360" plugin
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
            Architecture::registerArchitecture(Architecture::instance());
            Architecture::instance()->addVirtualMachineTemplate(H40VirtualMachineTemplate::instance());

            core::ComponentType::registerComponentType(Processor::Type::instance());
            core::ComponentType::registerComponentType(Storage::Type::instance());
            core::ComponentType::registerComponentType(Ibm2741::Type::instance());
        }
    };
}

using namespace ibm360;
BEGIN_PLUGIN_TABLE
    PLUGIN_TABLE_ITEM(Plugin)
END_PLUGIN_TABLE

//  End of emuone-ibm360/Plugin.cpp
