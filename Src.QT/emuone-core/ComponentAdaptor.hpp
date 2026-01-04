//
//  emuone-core/ComponentAdaptor.hpp - VM component adaptor
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
    /// \class IComponentAdaptor emuone-core/API.hpp
    /// \brief The VM component adaptor.
    /// \details
    ///     A ComponentAdaptor is never instatiated
    ///     directly. Instead, it is created when a component
    ///     is added to a VM whoose architecture is not directly
    ///     compatible woth that component.
    class EMUONE_CORE_PUBLIC IComponentAdaptor
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IComponentAdaptor)

        friend class VirtualMachine;

        //////////
        //  Types
    public:
        /// \brief
        ///     The component adaptor state.
        /// \details
        ///     Valid state transitions are:
        ///     -   Constructed <-> Connected.
        ///     -   Connected <-> Initialized.
        ///     -   Initialized <-> Running.
        enum class State
        {
            /// \brief
            ///     The ComponentAdaptor has been constructed and can be
            ///     bound or unbound. It is not connected to other
            ///     VM components and has no "runtime state".
            Constructed,

            /// \brief
            ///     The ComponentAdaptor is bound to (i.e. part of) a VM
            ///     and is connected to other required Components
            ///     of the same VM, but has no "runtime state".
            Connected,

            /// \brief
            ///     The ComponentAdaptor is bound to (i.e. part of) a VM,
            ///     is connected to other required Components of
            ///     the same VM and has a "runtime state", but is
            ///     inactive.
            Initialized,

            /// \brief
            ///     The ComponentAdaptor is bound to (i.e. part of) a VM,
            ///     is connected to other required Components of
            ///     the same VM, has a "runtime state", and is
            ///     active (i.e. currently operating).
            Running
        };

        //////////
        //  Construction/destruction
    protected:
        IComponentAdaptor(
                VirtualMachine * owner,
                IComponent * component
            );
        virtual ~IComponentAdaptor();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the small (16x16) icon representing this ComponentAdaptor.
        /// \return
        ///     The small (16x16) icon representing this ComponentAdaptor.
        virtual QIcon   smallIcon() const;

        /// \brief
        ///     Returns the large (32x32) icon representing this ComponentAdaptor.
        /// \return
        ///     The large (32x32) icon representing this ComponentAdaptor.
        virtual QIcon   largeIcon() const;

        /// \brief
        ///     Returns the type of this Component.
        /// \return
        ///     The type of this Component.
        virtual auto    type() const -> IComponentAdaptorType * = 0;

        /// \brief
        ///     Returns the VM to which this Component is bound.
        /// \return
        ///     The VM to which this ComponentAdaptor is bound; never nullptr.
        auto            owner() const -> VirtualMachine * { return _owner; }

        /// \brief
        ///     Returns the Component being adapted.
        /// \return
        ///     The Component being adapted; never nullptr.
        virtual auto    component() const -> IComponent * { return _component; }

        /// \brief
        ///     Creates a new editor widget for this component.
        /// \param parent
        ///     The parent for the new widget; nullptr == none.
        /// \returns
        ///     The newly created editor widget; nullptr if this
        ///     ComponentAdaptor requires no editor.
        virtual auto    createEditor(QWidget * parent) -> ComponentAdaptorEditor * = 0;

        //////////
        //  Operations (configuration)
    public:
        /// \brief
        ///     Checks whether the current configuration of this
        ///     component adaptor is valid.
        /// \details
        ///     A component adaptor will not permit setting invalid configuration
        ///     properties via confoguration-affecting methods; however,
        ///     that's no defense against a corrupt VM configuration
        ///     file which has been manually altered.
        /// \return
        ///     True the current configuration of this component
        ///     adaptor is valid, else false.
        virtual bool    isConfigurationValid() const = 0;

        /// \brief
        ///     Saves the configuration of this component adaptor by
        ///     setting the attributes of an XML DOM element as needed.
        /// \details
        ///     An overriding implementation in the derived class
        ///     MUST call the base class' implementation.
        /// \param element
        ///     The XML DOM element to modify.
        virtual void    saveConfiguration(QDomElement & element) const;

        /// \brief
        ///     Restores the configuration of this component adaptor by analyzing
        ///     the attributes of an XML DOM element as needed.
        /// \details
        ///     Any component adaptor configuration item that is not specified by
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
        ///     Returns the State of this ComponentAdaptor.
        /// \details
        ///     Can be safely called from any thread.
        /// \return
        ///     The State of this ComponentAdaptor.
        virtual State   state() const = 0;

        /// \brief
        ///     Performs the Constructed -> Connected state
        ///     change for this Component, connecting it to other
        ///     relevant Components of the same VM.
        /// \details
        ///     If the ComponentAdaptor is not in the Constructed state,
        ///     the call is an error.
        /// \exception VirtualMachineException
        ///     If an error occurs; the component adaptor state remains
        ///     unchanged when an exception is thrown.
        virtual void    connect() = 0;

        /// \brief
        ///     Performs the Connected -> Constructed state
        ///     change for this ComponentAdaptor, disonnecting it from
        ///     other Components of the same VM.
        /// \details
        ///     If the ComponentAdaptor is not in the Connected state,
        ///     the call has no effect.
        virtual void    disconnect() noexcept = 0;

        /// \brief
        ///     Performs the Connected -> Initialized state
        ///     change for this ComponentAdaptor, allocating its
        ///     "runtime state".
        /// \details
        ///     If the ComponentAdaptor is not in the Connected state,
        ///     the call is an error.
        /// \exception VirtualMachineException
        ///     If an error occurs; the component state remains
        ///     unchanged when an exception is thrown.
        virtual void    initialize() = 0;

        /// \brief
        ///     Performs the Initialized -> Connected state
        ///     change for this ComponentAdaptor, deallocating its
        ///     "runtime state".
        /// \details
        ///     If the ComponentAdaptor is not in the Initialized state,
        ///     the call has no effect.
        virtual void    deinitialize() noexcept = 0;

        /// \brief
        ///     Performs the Initialized -> Running state
        ///     change for this ComponentAdaptor, starting its
        ///     normal operation.
        /// \details
        ///     If the ComponentAdaptor is not in the Initialized state,
        ///     the call is an error.
        /// \exception VirtualMachineException
        ///     If an error occurs; the component state remains
        ///     unchanged when an exception is thrown.
        virtual void    start() = 0;

        /// \brief
        ///     Performs the Running -> Initialized state
        ///     change for this ComponentAdaptor, stopping its
        ///     normal operation.
        /// \details
        ///     If the ComponentAdaptor is not in the Running or Suspended state,
        ///     the call has no effect.
        virtual void    stop() noexcept = 0;

    protected:
        /// \brief
        ///     The guard to use for synchronizing all state changes.
        mutable emuone::util::Mutex guard;

        //////////
        //  Implementation
    private:
        VirtualMachine *const   _owner; //  mever nullptr
        IComponent *const       _component; //  being adapted
    };
}

//  End of emuone-core/ComponentAdaptor.hpp
