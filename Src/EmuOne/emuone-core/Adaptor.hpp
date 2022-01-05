//
//  emuone-core/Adaptor.hpp
//
//  A generic VM Component Adaptor
//
//////////
#pragma once

namespace core
{
    //////////
    //  Represents a type of an adaptor; also acts as a factory for adaptors of that type.
    class EMUONE_CORE_EXPORT AdaptorType : public util::StockObject
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AdaptorType)

        //////////
        //  Construction/destruction
    public:
        AdaptorType() {}
        virtual ~AdaptorType() {}

        //////////
        //  util::StockObject
    public:
        virtual QIcon       smallIcon() const override;
        virtual QIcon       largeIcon() const override;

        //////////
        //  Operations
    public:
        //  Checks if adaptors of this type are "compatible with" (i.e. can be
        //  used directly by) virtual appliances of the specified "architecture"
        virtual bool        isCompatibleWith(Architecture * architecture) const = 0;

        //  Checks if adaptors of this type are "compatible with" (i.e. can
        //  "adapt") components of a specified type to the tartet Architecture.
        virtual bool        isCompatibleWith(ComponentType * componentType) const = 0;

        //  Creates a new Adaptor of this type that "adapts" (wraps) the specified
        //  Component.
        //  Throws "VirtualApplianceException" if an error occurs (e.g. if the
        //  "component" is not compatible with this AdaptorType, etc.)
        virtual Adaptor *   createAdaptor(Component * adaptedComponent) = 0;

        //////////
        //  Registry
    public:
        static AdaptorTypeList      all();
        static AdaptorType *        findByMnemonic(const QString & mnemonic);
        static bool                 registerAdaptorType(AdaptorType * adaptorType);

        //  Finds an adaptor that can "adapt" components of the specified type to
        //  the specified "architecture"; returns nullptr if not found
        static AdaptorType *        find(Architecture * architecture, ComponentType * componentType);

    private:
        static AdaptorTypeList      _registry;
    };

    //////////
    //  A generic VM component adaptor
    class EMUONE_CORE_EXPORT Adaptor
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Adaptor)

        //////////
        //  Types
    public:
        //  Possible states of an Adaptor
        enum class State
        {
            Constructed,    //  Adaptor is constructed, but not connected to other Components, has no "runtime state" and is not "running"
            Connected,      //  Adaptor is constructed and connected to other Components, but has no "runtime state" and is not "running"
            Initialised,    //  Adaptor is constructed, connected to other Components and has "runtime state" but is not "running"
            Running         //  Adaptor is constructed, connected to other Components, has "runtime state" and is "running"
        };

        //////////
        //  Construction/destruction
    public:
        //  Constructs the adaptor.
        //  Throws "VirtualApplianceException" if an error occurs (e.g. if the
        //  "component" is not compatible with this AdaptorType, etc.)
        Adaptor(const QString & name, Component * adaptedComponent);
        virtual ~Adaptor();

        //////////
        //  Operations
    public:
        //  Checks if the specified string is valid as a "name" of an Adaptort
        static bool         isValidName(const QString & name);

        //  Returns the type of this adaptor.
        virtual AdaptorType *   type() const = 0;

        //  Returns the name of this adaptor.
        QString             name() const { return _name; }

        //  Sets the name of this adaptor.
        //  Throws "VirtualApplianceException" if the new "name" is not a valid adaptor name.
        void                setName(const QString & name);

        //  Creates a new "editor" widget for editing properties of this Adaptor.
        //  Returns "nullptr" if this adaptor has no properties to edit.
        virtual AdaptorEditor * createEditor(QWidget * parent) = 0;

        //  Returns a short message describing this Adaptor's status
        virtual QString     shortStatus() const = 0;

        //  Returns the VA to which this Adaptor belongs (never nullptr).
        VirtualAppliance *  virtualAppliance() const { return _virtualAppliance; }

        //  Creates a new UI for this Adaptor; returns nullptr if this Adaptor has no UI
        virtual AdaptorUi * createUi() = 0;

        //////////
        //  Operations (state control) - all thread-safe
    public:
        virtual State       state() const = 0;
        virtual void        connect() = 0;      //  throws VirtualApplianceException
        virtual void        initialise() = 0;   //  throws VirtualApplianceException;
        virtual void        start() = 0;        //  throws VirtualApplianceException
        virtual void        stop() noexcept = 0;
        virtual void        deinitialise() noexcept = 0;
        virtual void        disconnect() noexcept = 0;

        //////////
        //  Operations (serialisation)
    public:
        //  Serialises this Adaptor's configuration by setting the
        //  "configurationElement"'s attributes appropriately
        virtual void        serialiseConfiguration(QDomElement & configurationElement) const = 0;

        //  De-serialises this Adaptor's configuration by analysing the
        //  "configurationElement"'s attributes as necessary
        virtual void        deserialiseConfiguration(QDomElement & configurationElement) = 0;

        //////////
        //  Operations
    public:
        //  The component wrapped by ("adaptd by") this Adaptor.
        Component *         adaptedComponent() const { return _adaptedComponent; }

        //////////
        //  Implementation
    private:
        QString             _name;
        Component *         _adaptedComponent;
        VirtualAppliance *  _virtualAppliance = nullptr;    //  ...to which this adaptor belongs; never nullptr
    };
}

//  End of emuone-core/Adaptor.hpp
