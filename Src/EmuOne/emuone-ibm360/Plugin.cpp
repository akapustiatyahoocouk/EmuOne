//
//  emuone-ibm360/Plugin.cpp
//
//  The emuone-ibm360 plugin
//
//////////
#include "emuone-ibm360/API.hpp"

//////////
//  The "emuone-ibm360" plugin
class Ibm360Plugin : public Plugin
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Ibm360Plugin)

    //////////
    //  Construction/destruction
public:
    Ibm360Plugin() {}
    virtual ~Ibm360Plugin() {}

    //////////
    //  Plugin
public:
    virtual void initialise()
    {
        Architecture::registerArchitecture(Ibm360Architecture::getInstance());

        Ibm360Architecture::getInstance()->addVirtualMachineTemplate(Ibm360H40VirtualMachineTemplate::getInstance());

        ComponentType::registerComponentType(Ibm360Storage::Type::getInstance());
    }
};

BEGIN_PLUGIN_TABLE
    PLUGIN_TABLE_ITEM(Ibm360Plugin)
END_PLUGIN_TABLE

//  End of emuone-ibm360/Plugin.cpp
