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
    //  Represents a general "category" of components (e.g. "storage devices", "terminals", etc.
    //  Concrete derived classes will normally be singletons registered by plugins.
    class EMUONE_CORE_EXPORT ComponentCategory : public util::StockObject
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ComponentCategory)

        //////////
        //  Construction/destruction
    public:
        ComponentCategory() {}
        virtual ~ComponentCategory() {}

        //////////
        //  Operations
    public:
        //  Returns an unordered list of all known component types that belong
        //  to this component category
        ComponentTypeList   componentTypes() const;

        //////////
        //  Registry
    public:
        static ComponentCategoryList    all();
        static ComponentCategory *      findByMnemonic(const QString & mnemonic);
        static bool                     registerComponentCategory(ComponentCategory * componentCategory);
    private:
        static ComponentCategoryList    _registry;
    };

    //////////
    //  The "standard" (predefined) component categories
    namespace StandardComponentCategories
    {
        //////////
        //  Types
        class EMUONE_CORE_EXPORT Memory : public ComponentCategory
        {
            DECLARE_SINGLETON(Memory)

            //////////
            //  StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;

            //////////
            //  Implementation
        private:
            mutable QIcon       _smallIcon = QIcon();
            mutable QIcon       _largeIcon = QIcon();
        };

        class EMUONE_CORE_EXPORT Processors : public ComponentCategory
        {
            DECLARE_SINGLETON(Processors)

            //////////
            //  StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;

            //////////
            //  Implementation
        private:
            mutable QIcon       _smallIcon = QIcon();
            mutable QIcon       _largeIcon = QIcon();
        };

        class EMUONE_CORE_EXPORT Motherboard : public ComponentCategory
        {
            DECLARE_SINGLETON(Motherboard)

            //////////
            //  StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;

            //////////
            //  Implementation
        private:
            mutable QIcon       _smallIcon = QIcon();
            mutable QIcon       _largeIcon = QIcon();
        };

        class EMUONE_CORE_EXPORT Terminals : public ComponentCategory
        {
            DECLARE_SINGLETON(Terminals)

            //////////
            //  StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;

            //////////
            //  Implementation
        private:
            mutable QIcon       _smallIcon = QIcon();
            mutable QIcon       _largeIcon = QIcon();
        };

        class EMUONE_CORE_EXPORT Software : public ComponentCategory
        {
            DECLARE_SINGLETON(Software)

            //////////
            //  StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;

            //////////
            //  Implementation
        private:
            mutable QIcon       _smallIcon = QIcon();
            mutable QIcon       _largeIcon = QIcon();
        };
    };

    //////////
    //  Represents a type of a component; also acts as a factory for components of that type.
    class EMUONE_CORE_EXPORT ComponentType : public util::StockObject
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ComponentType)

        //////////
        //  Construction/destruction
    public:
        ComponentType() {}
        virtual ~ComponentType() {}

        //////////
        //  util::StockObject
    public:
        virtual QIcon       smallIcon() const override { return category()->smallIcon(); }
        virtual QIcon       largeIcon() const override { return category()->largeIcon(); }

        //////////
        //  Operations
    public:
        //  Returns the category to which this component type belongs.
        virtual ComponentCategory * category() const = 0;

        //  Checks if components of this type are "compatible with" (i.e. can be
        //  used directly by) virtual appliances of the specified "architecture"
        virtual bool                isCompatibleWith(const Architecture * architecture) const = 0;

        //  Checks if components of this type are "adaptable to" (i.e. can be used
        //  indirectly via "adaptors") by virtual appliances of the specified "architecture"
        bool                        isAdaptableTo(const Architecture * architecture) const;

        //  Creates and returns a new Component of this type with default configuration.
        //  Throws VirtualApplianceException if the component creation fails.
        virtual Component *         createComponent() = 0;

        //////////
        //  Registry
    public:
        static ComponentTypeList    all();
        static ComponentType *      findByMnemonic(const QString & mnemonic);
        static bool                 registerComponentType(ComponentType * componentType);
    private:
        static ComponentTypeList    _registry;
    };

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
        //  Checks if the specified string is valid as a "name" of a Component
        static bool         isValidName(const QString & name);

        //  Returns the type of this component.
        virtual ComponentType * type() const = 0;

        //  Returns the name of this component.
        QString             name() const { return _name; }

        //  Sets the name of this component.
        //  Throws "VirtualApplianceException" if the new "name" is not a valid component name.
        void                setName(const QString & name);

        //  Creates a new "editor" widget for editing properties of this Component.
        //  Returns "nullptr" if this component has no properties to edit.
        virtual ComponentEditor *   createEditor(QWidget * parent) = 0;

        //  Returns a short message describing this Component's status
        virtual QString     shortStatus() const = 0;

        //  Returns the VA to which this Component currently belongs; nullptr == standalone
        VirtualAppliance *  virtualAppliance() const { return _virtualAppliance; }

        //  Creates a new UI for this Component; returns nullptr if this Component has no UI
        virtual ComponentUi *   createUi() = 0;

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
    //  A "component aspect" is a feature interface that a Component (or an Adaptor)
    //  can implement if it is capable of a specific set of services.
    //  A concrete Component-derived (or Adaptor-derived) class may choose to also
    //  "public virtual"ly derive from any number of "IComponentAspect"s.
    class EMUONE_CORE_EXPORT IComponentAspect
    {
        //////////
        //  This is an interface
    public:
        virtual ~IComponentAspect() {}
    };

    //////////
    //  An aspect of a Component that is driven by a clock
    class EMUONE_CORE_EXPORT IClockedComponentAspect : public virtual IComponentAspect
    {
        //////////
        //  This is an interface
    public:
        virtual ~IClockedComponentAspect() {}

        //////////
        //  Operations
    public:
        //  The clock frequency of this component
        virtual ClockFrequency  clockFrequency() const = 0;

        //  Called on every clock tick when the component is "running"
        virtual void            onClockTick() = 0;
    };
}

//  End of emuone-core/Component.hpp
