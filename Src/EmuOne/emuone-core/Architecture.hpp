//
//  emuone-core/Architecture.hpp
//
//  The VM architecture
//
//////////
#pragma once

//////////
//  Represents a VM architecture.
class EMUONE_CORE_EXPORT Architecture : public StockObject
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Architecture)

    //////////
    //  Construction/destruction
public:
    Architecture() {}
    virtual ~Architecture() {}

    //////////
    //  Operations
public:
    bool                        addVirtualApplianceTemplate(VirtualApplianceTemplate * virtualApplianceTemplate);
    bool                        addVirtualMachineTemplate(VirtualMachineTemplate * virtualMachineTemplate);
    bool                        addRemoteTerminalTemplate(RemoteTerminalTemplate * remoteTerminalTemplate);

    VirtualApplianceTemplateList    getVirtualApplianceTemplates() const { return _virtualApplianceTemplates; }
    VirtualMachineTemplateList      getVirtualMachineTemplates() const { return _virtualMachineTemplates; }
    RemoteTerminalTemplateList      getRemoteTerminalTemplates() const { return _remoteTerminalTemplates; }

    VirtualApplianceTemplate *  findVirtualApplianceTemplate(const QString & mnemonic) const;
    VirtualMachineTemplate *    findVirtualMachineTemplate(const QString & mnemonic) const;
    RemoteTerminalTemplate *    findRemoteTerminalTemplate(const QString & mnemonic) const;

    //////////
    //  Registry
public:
    static ArchitectureList     getAll();
    static Architecture *       findByMnemonic(const QString & mnemonic);
    static bool                 registerArchitecture(Architecture * architecture);
private:
    static ArchitectureList     _registry;

    //////////
    //  Implementation
private:
    VirtualApplianceTemplateList    _virtualApplianceTemplates;
    VirtualMachineTemplateList      _virtualMachineTemplates;
    RemoteTerminalTemplateList      _remoteTerminalTemplates;
};

//  End of emuone-core/Architecture.hpp
