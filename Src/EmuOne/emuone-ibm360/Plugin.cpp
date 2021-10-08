//
//  emuone-ibm360/Ibm360Plugin.cpp
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
            Architecture::registerArchitecture(Architecture::getInstance());
            Architecture::getInstance()->addVirtualMachineTemplate(H40VirtualMachineTemplate::getInstance());

            core::ComponentType::registerComponentType(Processor::Type::getInstance());
            core::ComponentType::registerComponentType(Storage::Type::getInstance());
            core::ComponentType::registerComponentType(Ibm2741::Type::getInstance());
        }
    };
}

using namespace ibm360;
BEGIN_PLUGIN_TABLE
    PLUGIN_TABLE_ITEM(Plugin)
END_PLUGIN_TABLE

//  End of emuone-ibm360/Ibm360Plugin.cpp
