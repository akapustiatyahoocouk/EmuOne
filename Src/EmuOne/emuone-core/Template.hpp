//
//  emuone-core/Template.hpp
//
//  The VA templates
//
//////////
#pragma once

//////////
//  Represents a generic VA template
class EMUONE_CORE_EXPORT VirtualApplianceTemplate : public StockObject
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(VirtualApplianceTemplate)

    //////////
    //  Construction/destruction
public:
    VirtualApplianceTemplate() {}
    virtual ~VirtualApplianceTemplate() {}
};

//////////
//  Represents a generic VM template
class EMUONE_CORE_EXPORT VirtualMachineTemplate : public VirtualApplianceTemplate
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(VirtualMachineTemplate)

    //////////
    //  Construction/destruction
public:
    VirtualMachineTemplate() {}
    virtual ~VirtualMachineTemplate() {}
};

//////////
//  Represents a generic RT template
class EMUONE_CORE_EXPORT RemoteTerminalTemplate : public VirtualApplianceTemplate
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RemoteTerminalTemplate)

    //////////
    //  Construction/destruction
public:
    RemoteTerminalTemplate() {}
    virtual ~RemoteTerminalTemplate() {}
};

//  End of emuone-core/Template.hpp
