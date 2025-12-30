//
//  emuone-core/Components.hpp - VM component
//
//  EmuOne
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////

namespace emuone::core
{
    /// \class IComponent emuone-core/API.hpp
    /// \brief The VM component.
    /// \details
    ///     A Component can be bound to a VM or unbound;
    ///     all Components start off unbound.
    ///     When a Component is bound to a VM, the VM takes
    ///     ownership of this Component; in particular, the
    ///     Component cannot be delete'd unless the entire VM
    ///     is delete'd.
    class EMUONE_CORE_PUBLIC IComponent
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IComponent)

        friend class VirtualMachine;

        //////////
        //  Types
    public:
        /// \brief
        ///     The component state.
        /// \details
        ///     Valid state transitions are:
        ///     -   Constructed <-> Connected.
        ///     -   Connected <-> Initialized.
        ///     -   Initialized <-> Running.
        enum class State
        {
            /// \brief
            ///     The Component has been constructed and can be
            ///     bound or unbound. It is not connected to other
            ///     VM components and has no "runtime state".
            Constructed,

            /// \brief
            ///     The Component is bound to (i.e. part of) a VM
            ///     and is connected to other required Components
            ///     of the same VM, but has no "runtime state".
            Connected,

            /// \brief
            ///     The Component is bound to (i.e. part of) a VM,
            ///     is connected to other required Components of
            ///     the same VM and has a "runtime state", but is
            ///     inactive.
            Initialized,

            /// \brief
            ///     The Component is bound to (i.e. part of) a VM,
            ///     is connected to other required Components of
            ///     the same VM, has a "runtime state", and is
            ///     active (i.e. currently operating).
            Running
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Costructs an initiallz unbound component.
        IComponent() = default;

        /// \brief
        ///     The class destructor.
        /// \details
        ///     The Component must be unbound or an error occurs.
        virtual ~IComponent() { Q_ASSERT(_owner == nullptr); };

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the small (16x16) icon representing this Component.
        /// \return
        ///     The small (16x16) icon representing this Component.
        virtual QIcon   smallIcon() const;

        /// \brief
        ///     Returns the large (32x32) icon representing this Component.
        /// \return
        ///     The large (32x32) icon representing this Component.
        virtual QIcon   largeIcon() const;

        /// \brief
        ///     Returns the type of this Component.
        /// \return
        ///     The type of this Component.
        virtual auto    type() const -> IComponentType * = 0;

        /// \brief
        ///     Returns the user-readable display name of this Component.
        /// \details
        ///     This typically includes component type's display name and, maybe,
        ///     one or two of the most important component configuration items.
        /// \return
        ///     The user-readable display name of this Component.
        virtual QString displayName() const = 0;

        /// \brief
        ///     Returns the VM to which this Component is bound.
        /// \return
        ///     The VM to which this Component is bound; nullptr ==
        ///     Component is unbound.
        VirtualMachine *owner() const;

        /// \brief
        ///     Checks whether this Component is bound to a VM.
        /// \return
        ///     True if this Component is bound to a VM, false if not.
        bool            isBound() const { return owner() != nullptr; }

        /// \brief
        ///     Creates a new editor widget for this component.
        /// \param parent
        ///     The parent for the new widget; nullptr == none.
        /// \returns
        ///     The newly created editor widget; ; nullptr if this
        ///     Component requires no editor
        virtual auto    createEditor(QWidget * parent) -> ComponentEditor * = 0;

        /// \brief
        ///     Saves the configuration of this component by setting
        ///     the attributes of an XML DOM element as needed.
        /// \details
        ///     An overriding implementation in the derived class
        ///     MUST call the base class' implementation.
        /// \param element
        ///     The XML DOM element to modify.
        virtual void    saveConfiguration(QDomElement & element) const;

        /// \brief
        ///     Restores the configuration of this component by analyzing
        ///     the attributes of an XML DOM element as needed.
        /// \details
        ///     Any component configuration item that is not specified by
        ///     the XML DOM element or is specified incorrectly will
        ///     retain its value intact.
        ///     An overriding implementation in the derived class
        ///     MUST call the base class' implementation.
        /// \param element
        ///     The XML DOM element to analyze.
        virtual void    restoreConfiguration(const QDomElement & element);

        //////////
        //  Operations (state control)
    public:
        /// \brief
        ///     Returns the State of this Component.
        /// \details
        ///     Can be safely called from any thread.
        /// \return
        ///     The State of this Component.
        virtual State   state() const = 0;

        /// \brief
        ///     Performs the Constructed -> Connected state
        ///     change for this Component, connecting it to other
        ///     relevant Components of the same VM.
        /// \details
        ///     If the Component is not in the Constructed state,
        ///     the call is an error.
        /// \exception VirtualMachineException
        ///     If an error occurs; the component state remains
        ///     unchanged when an exception is thrown.
        virtual void    connect() = 0;

        /// \brief
        ///     Performs the Connected -> Constructed state
        ///     change for this Component, disonnecting it from
        ///     other Components of the same VM.
        /// \details
        ///     If the Component is not in the Connected state,
        ///     the call has no effect.
        virtual void    disconnect() noexcept = 0;

        /// \brief
        ///     Performs the Connected -> Initialized state
        ///     change for this Component, allocating its
        ///     "runtime state".
        /// \details
        ///     If the Component is not in the Connected state,
        ///     the call is an error.
        /// \exception VirtualMachineException
        ///     If an error occurs; the component state remains
        ///     unchanged when an exception is thrown.
        virtual void    initialize() = 0;

        /// \brief
        ///     Performs the Initialized -> Connected state
        ///     change for this Component, deallocating its
        ///     "runtime state".
        /// \details
        ///     If the Component is not in the Initialized state,
        ///     the call has no effect.
        virtual void    deinitialize() noexcept = 0;

        /// \brief
        ///     Performs the Initialized -> Running state
        ///     change for this Component, starting its
        ///     normal operation.
        /// \details
        ///     If the Component is not in the Initialized state,
        ///     the call is an error.
        /// \exception VirtualMachineException
        ///     If an error occurs; the component state remains
        ///     unchanged when an exception is thrown.
        virtual void    start() = 0;

        /// \brief
        ///     Performs the Running -> Initialized state
        ///     change for this Component, stopping its
        ///     normal operation.
        /// \details
        ///     If the Component is not in the Running or Suspended state,
        ///     the call has no effect.
        virtual void    stop() noexcept = 0;

    protected:
        /// \brief
        ///     The guard to use for synchronizing all state changes.
        mutable emuone::util::Mutex guard;

        //////////
        //  Implementation
    private:
        VirtualMachine *_owner = nullptr;
    };

    /// \class IDevice emuone-core/API.hpp
    /// \brief A generally asynchronous message-driven Component.
    class EMUONE_CORE_PUBLIC IDevice
        :   public virtual IComponent
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IDevice)

        //////////
        //  Construction/destruction
    public:
        IDevice() = default;
        virtual ~IDevice() = default;

        //////////
        //  Operations
    public:
    };
}

//  End of emuone-core/Components.hpp
