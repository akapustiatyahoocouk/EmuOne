//
//  emuone-core/Component.hpp
//
//  A generic VM Component
//
//////////
#pragma once

namespace core
{
    //////////
    //  A generic VM component
    class EMUONE_CORE_EXPORT Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Component)

        friend class VirtualAppliance;

        //////////
        //  Types
    public:
        //  Possible states of a Component
        enum class State
        {
            Constructed,    //  Componet is constructed, but not connected to other Components, has no "runtime state" and is not "running"
            Connected,      //  Componet is constructed and connected to other Components, but has no "runtime state" and is not "running"
            Initialised,    //  Componet is constructed, connected to other Components and has "runtime state" but is not "running"
            Running         //  Componet is constructed, connected to other Components, has "runtime state" and is "running"
        };

        //////////
        //  Construction/destruction
    public:
        //  Constructs the component.
        //  Throws "VirtualApplianceException" if the new "name" is not a valid component name.
        explicit Component(const QString & name);
        virtual ~Component();

        //////////
        //  Operations
    public:
        static bool         isValidName(const QString & name);

        //  Returns the type of this component.
        virtual ComponentType * getType() const = 0;

        //  Returns the name of this component.
        QString             getName() const { return _name; }

        //  Sets the name of this component.
        //  Throws "VirtualApplianceException" if the new "name" is not a valid component name.
        void                setName(const QString & name);

        //  Creates a new "editor" widget for editing properties of this Component.
        //  Returns "nullptr" if this component has no properties to edit.
        virtual ComponentEditor *   createEditor(QWidget * parent) = 0;

        //  Returns a short message describing this Component's status
        virtual QString     getShortStatus() const = 0;

        //  Returns the VA to which this Component currently belongs; nullptr == standalone
        VirtualAppliance *  getVirtualAppliance() const { return _virtualAppliance; }

        //////////
        //  Operations (state control) - all thread-safe
    public:
        virtual State       getState() const = 0;
        virtual void        connect() = 0;      //  throws VirtualApplianceException
        virtual void        initialise() = 0;   //  throws VirtualApplianceException;
        virtual void        start() = 0;        //  throws VirtualApplianceException
        virtual void        stop() noexcept = 0;
        virtual void        deinitialise() noexcept = 0;
        virtual void        disconnect() noexcept = 0;

        //////////
        //  Operations (serialisation)
    public:
        //  Serialises this Component's configuration by setting the
        //  "configurationElement"'s attributes appropriately
        virtual void        serialiseConfiguration(QDomElement & configurationElement) const = 0;

        //  De-serialises this Component's configuration by analysing the
        //  "configurationElement"'s attributes as necessary
        virtual void        deserialiseConfiguration(QDomElement & configurationElement) = 0;

        //////////
        //  Implementation
    private:
        QString             _name;
        VirtualAppliance *  _virtualAppliance = nullptr;    //  ...to which this component belongs; null == nojne
    };

    //////////
    //  A special kind of "component" that adapts another Component to an unfamiliar Architecture
    class EMUONE_CORE_EXPORT Adaptor : public Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Adaptor)

        //////////
        //  Construction/destruction
    public:
        explicit Adaptor(Component * adaptedComponent);
        virtual ~Adaptor();

        //////////
        //  Operations
    public:
        Component *         getAdaptedComponent() const { return _adaptedComponent; }

        //////////
        //  Implementation
    private:
        Component *         _adaptedComponent;
    };
}

//  End of emuone-core/Component.hpp
