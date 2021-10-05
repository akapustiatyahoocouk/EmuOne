//
//  emuone-core/VirtualAppliance.hpp
//
//  The VirtualAppliance framework
//
//////////
#pragma once

//////////
//  A "type" of a virtual appliance
class EMUONE_CORE_EXPORT VirtualApplianceType : public StockObject
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(VirtualApplianceType)

    //////////
    //  Construction/destruction
public:
    VirtualApplianceType() {}
    virtual ~VirtualApplianceType() {}

    //////////
    //  Operations
public:
    //  Creates a new VA of this type, initially without any components
    virtual VirtualAppliance *  createVirtualAppliance(const QString & name, const QString & location, Architecture * architecture, VirtualApplianceTemplate * virtualApplianceTemplate) = 0;

    //////////
    //  Registry
public:
    static VirtualApplianceType *   findByMnemonic(const QString & mnemonic);
};

//////////
//  A generic VA
class EMUONE_CORE_EXPORT VirtualAppliance
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(VirtualAppliance)

    friend class VirtualMachine;
    friend class RemoteTerminal;

    //////////
    //  Constants
public:
    //  The preferred extension for VA configuration files (without leading '.')
    static const QString    PreferredExtension;

    //////////
    //  Types
public:
    //  Possible VA states
    enum class State
    {
        Stopped,    //  no runtime state, no saved state, not running
        Running,    //  runtime state exists, no saved state, running
        Suspended   //  no runtime state, saved state exists, not running
    };

    //////////
    //  Construction/destruction
public:
    VirtualAppliance(const QString & name, const QString & location, Architecture * architecture, VirtualApplianceTemplate * virtualApplianceTemplate);
    virtual ~VirtualAppliance();

    //////////
    //  Operations
public:
    static bool                 isValidName(const QString & name);

    virtual VirtualApplianceType *  getType() const = 0;

    QString                     getName() const { return _name; }
    void                        setName(const QString & name);
    QString                     getLocation() { return _location; }
    Architecture *              getArchitecture() const { return _architecture; }
    virtual VirtualApplianceTemplate *  getTemplate() const { return _template; }

    //  Saves tis VA's configuration to the "location" XML file.
    //  Throws VirtualApplianceException if an error occurs.
    void                        save() const;

    //  Loads a VA's configuration from the "location" XML file.
    //  Throws VirtualApplianceException if an error occurs.
    static VirtualAppliance *   load(const QString & location);

    //  Adds the specified "component" to this VA.
    //  Throws VirtualApplianceException if the componentn cannot be added.
    virtual void                addComponent(Component * component);

    //  Remove the specified "component" from this VA (if possible).
    //  Throws VirtualApplianceException if the componentn cannot be removed.
    virtual void                removeComponent(Component * component);

    //////////
    //  Operations (state control)
public:
    State                       getState() const;
    void                        start();    //  throws VirtualApplianceException
    void                        stop();
    void                        suspend();  //  throws VirtualApplianceException
    void                        resume();   //  throws VirtualApplianceException

    //////////
    //  Implementation
private:
    QString                     _name;
    const QString               _location;      //  always the full path to a .VA file
    Architecture *const         _architecture;  //  ...of this VA
    VirtualApplianceTemplate *const _template;      //  ...from which this VA was created

    State                       _state = State::Stopped;
    mutable QRecursiveMutex     _stateGuard;

    ComponentList               _components;    //  "add()"'ed to this VA
    ComponentList               _compatibleComponents;  //  These "_components" which are directly compatible with this VA
    ComponentList               _adaptedComponents;     //  These "_components" which had to be "adapted" to this this VA
    AdaptorList                 _adaptors;              //  One for each of "_adaptedComponents"
};

//////////
//  A "generic VA"virtual machine" is an emulation of some computer running locally
class EMUONE_CORE_EXPORT VirtualMachine : public VirtualAppliance
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(VirtualMachine)

    //////////
    //  Types
public:
    class Type : public VirtualApplianceType
    {
        DECLARE_SINGLETON(Type)

        //////////
        //  StockObject
    public:
        virtual QString     getMnemonic() const override;
        virtual QString     getDisplayName() const override;
        virtual QIcon       getSmallIcon() const override;
        virtual QIcon       getLargeIcon() const override;

        //////////
        //  VirtualApplianceType
    public:
        virtual VirtualMachine *    createVirtualAppliance(const QString & name, const QString & location, Architecture * architecture, VirtualApplianceTemplate * virtualApplianceTemplate) override;

        //////////
        //  Implementation
    private:
        mutable QIcon       _smallIcon;
        mutable QIcon       _largeIcon;
    };

    //////////
    //  Construction/destruction
public:
    VirtualMachine(const QString & name, const QString & location, Architecture * architecture, VirtualMachineTemplate * virtualMachineTemplate);
    virtual ~VirtualMachine();

    //////////
    //  VirtualAppliance
public:
    virtual VirtualApplianceType *  getType() const override;
    virtual VirtualMachineTemplate *getTemplate() const override;
};

//////////
//  A "remote terminal" connects to a VM running remotely and acts as a terminal
class EMUONE_CORE_EXPORT RemoteTerminal : public VirtualAppliance
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RemoteTerminal)

    //////////
    //  Types
public:
    class Type : public VirtualApplianceType
    {
        DECLARE_SINGLETON(Type)

        //////////
        //  StockObject
    public:
        virtual QString     getMnemonic() const override;
        virtual QString     getDisplayName() const override;
        virtual QIcon       getSmallIcon() const override;
        virtual QIcon       getLargeIcon() const override;

        //////////
        //  VirtualApplianceType
    public:
        virtual RemoteTerminal *    createVirtualAppliance(const QString & name, const QString & location, Architecture * architecture, VirtualApplianceTemplate * virtualApplianceTemplate) override;

        //////////
        //  Implementation
    private:
        mutable QIcon       _smallIcon;
        mutable QIcon       _largeIcon;
    };

    //////////
    //  Construction/destruction
public:
    RemoteTerminal(const QString & name, const QString & location, Architecture * architecture, RemoteTerminalTemplate * remoteTerminalTemplate);
    virtual ~RemoteTerminal();

    //////////
    //  VirtualAppliance
public:
    virtual VirtualApplianceType *  getType() const override;
    virtual RemoteTerminalTemplate *getTemplate() const override;
};

//  End of emuone-core/VirtualAppliance.hpp
