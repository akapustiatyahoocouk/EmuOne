//
//  emuone-core/VirtualAppliance.hpp
//
//  The VirtualAppliance framework
//
//////////
#pragma once

//////////
//  A generic VA
class EMUONE_CORE_EXPORT VirtualAppliance
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(VirtualAppliance)

    //////////
    //  Construction/destruction
public:
    VirtualAppliance(const QString & name, const QString & location, Architecture * architecture, VirtualApplianceTemplate * virtualApplianceTemplate);
    virtual ~VirtualAppliance();

    //////////
    //  Operations
public:
    static bool                 isValidName(const QString & name);

    QString                     getName() const { return _name; }
    void                        setName(const QString & name);
    QString                     getLocation() { return _location; }
    Architecture *              getArchitecture() const { return _architecture; }
    VirtualApplianceTemplate *  getTemplate() const { return _template; }

    //////////
    //  Implementation
private:
    QString                     _name;
    const QString               _location;      //  always the full path to a .VA file
    Architecture *const         _architecture;  //  ...of this VA
    VirtualApplianceTemplate *const _template;      //  ...from which this VA was created
};

//////////
//  A "generic VA"virtual machine" is an emulation of some computer running locally
class EMUONE_CORE_EXPORT VirtualMachine : public VirtualAppliance
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(VirtualMachine)

    //////////
    //  Construction/destruction
public:
    VirtualMachine(const QString & name, const QString & location, Architecture * architecture, VirtualMachineTemplate * virtualMachineTemplate);
    virtual ~VirtualMachine();

    //////////
    //  VirtualAppliance
public:
    VirtualMachineTemplate *  getTemplate() const { return dynamic_cast<VirtualMachineTemplate*>(VirtualAppliance::getTemplate()); }
};

//////////
//  A "remote terminal" connects to a VM running remotely and acts as a terminal
class EMUONE_CORE_EXPORT RemoteTerminal : public VirtualAppliance
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RemoteTerminal)

    //////////
    //  Construction/destruction
public:
    RemoteTerminal(const QString & name, const QString & location, Architecture * architecture, RemoteTerminalTemplate * remoteTerminalTemplate);
    virtual ~RemoteTerminal();

    //////////
    //  VirtualAppliance
public:
    RemoteTerminalTemplate *  getTemplate() const { return dynamic_cast<RemoteTerminalTemplate*>(VirtualAppliance::getTemplate()); }
};

//  End of emuone-core/VirtualAppliance.hpp
