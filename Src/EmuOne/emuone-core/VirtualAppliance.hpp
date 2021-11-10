//
//  emuone-core/VirtualAppliance.hpp
//
//  The VirtualAppliance framework
//
//////////
#pragma once

namespace core
{
    //////////
    //  A "type" of a virtual appliance
    class EMUONE_CORE_EXPORT VirtualApplianceType : public util::StockObject
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

        //  Returns the type of this Virtual Appliance
        virtual VirtualApplianceType *  type() const = 0;

        //  Returns/sets the type of this Virtual Appliance
        QString                     name() const { return _name; }
        void                        setName(const QString & name);

        //  Returns the "type "location" of this Virtual Appliance (this is the full
        //  path of the VA's configuration file)
        QString                     location() { return _location; }

        //  Returns the Architecture of this Virtual Appliance
        Architecture *              architecture() const { return _architecture; }

        //  Returns the Template from which this Virtual Appliance has been initially created
        virtual VirtualApplianceTemplate *  initialTemplate() const { return _template; }

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

        ComponentList               components() const;
        ComponentList               components(ComponentCategory * componentCategory);
        AdaptorList                 adaptors() const { return _adaptors; }
        AdaptorList                 adaptors(ComponentCategory * componentCategory);

        //  Returns an unordered list of all a) compatible components and
        //  b) adaptors which implement the specified interface T
        template <class T>
        QList<T*>                   findComponentsByRole() const;

        //  *   If the "path" is relative, returns it "as is".
        //  *   If the "path" is absolute, attempts to express it as relative to
        //      this VA's "location" directory; returns "path" if that cannot be done.
        QString                     toRelativePath(const QString & path) const;

        //  *   If the "path" is relative, returns its absolute form taking
        //      this VA's location directory as base.
        //  *   If the "path" is absolute, returns it "as is".
        QString                     toAbsolutePath(const QString & path) const;

        //  Operations (state control) - all thread-safe
    public:
        State                       state() const;
        void                        start();    //  throws VirtualApplianceException
        void                        stop() noexcept;
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

        ComponentList               _components;            //  "add()"'ed to this VA
        ComponentList               _compatibleComponents;  //  These "_components" which are directly compatible with this VA
        ComponentList               _adaptedComponents;     //  These "_components" which had to be "adapted" to this this VA
        AdaptorList                 _adaptors;              //  One for each of "_adaptedComponents"

        //  Helpers
        void                        _connectComponents();
        void                        _initialiseComponents();
        void                        _startComponents();
        void                        _stopComponents();
        void                        _deinitialiseComponents();
        void                        _disconnectComponents();
    };

    template <class T>
    QList<T*> VirtualAppliance::findComponentsByRole() const
    {
        QList<T*> result;
        for (Component * component : _compatibleComponents)
        {
            if (T * t = dynamic_cast<T*>(component))
            {
                result.append(t);
            }
        }
        for (Adaptor * adaptor : _adaptors)
        {
            if (T * t = dynamic_cast<T*>(adaptor))
            {
                result.append(t);
            }
        }
        return result;
    }

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
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;

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
        virtual VirtualApplianceType *  type() const override;
        virtual VirtualMachineTemplate *initialTemplate() const override;
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
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;

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
        virtual VirtualApplianceType *  type() const override;
        virtual RemoteTerminalTemplate *initialTemplate() const override;
    };
}

//  End of emuone-core/VirtualAppliance.hpp
