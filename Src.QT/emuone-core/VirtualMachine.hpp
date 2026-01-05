//
//  emuone-core/VirtualMachine.hpp - A complete VM
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
#pragma once
#include "emuone-core/API.hpp"

namespace emuone::core
{
    /// \class VirtualMachine emuone-core/API.hpp
    /// \brief The complete VM.
    class EMUONE_CORE_PUBLIC VirtualMachine
        : public QObject
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(VirtualMachine)

        //////////
        //  Constants
    public:
        /// \brief
        ///     The preferred extension for EmuOne VM configuration files.
        inline static const QString PreferredExtension = ".emuone";

        /// \brief
        ///     The preferred extension for EmuOne VM saved state files.
        inline static const QString SavedStateExtension = ".sst";

        //////////
        //  Types
    public:
        /// \brief
        ///     The VM state.
        enum class State
        {
            /// \brief
            ///     The VM has no runtime state and is not running.
            ///     A saved VM runtime state does not exist.
            Stopped,

            /// \brief
            ///     The VM has full runtime state and is running.
            ///     A saved VM runtime state does not exist.
            Running,

            /// \brief
            ///     The VM has no runtime state and is not running.
            ///     A saved VM runtime state exists.
            Suspended
        };

        //////////
        //  Construction/destruction
    protected:
        VirtualMachine( //  used by deserialization
                const QString & name,
                const QString & location,
                IArchitecture * architecture,
                IVirtualMachineType * type,
                IVirtualMachineTemplate * createdFrom
            );
    protected:
        /// \brief
        ///     Constructs an empty VM (with no components).
        /// \param name
        ///     The VM name.
        /// \param location
        ///     The VM location (a full path to the VM configuration file).
        /// \param architecture
        ///     The VM architecture; cannot be nullpyr.
        /// \param type
        ///     The VM type; cannot be nullpyr.
        VirtualMachine(
                const QString & name,
                const QString & location,
                IArchitecture * architecture,
                IVirtualMachineType * type
            );

        /// \brief
        ///     Constructs the VM from VM template.
        /// \param name
        ///     The VM name.
        /// \param location
        ///     The VM location (a full path to the VM configuration file).
        /// \param createdFrom
        ///     The template frpom which the VM was created; can be nullptr.
        VirtualMachine(
                const QString & name,
                const QString & location,
                IVirtualMachineTemplate * createdFrom
            );

    public:
        /// \brief
        ///     The class destructor.
        /// \details
        ///     If the VM is currently Running, it is
        ///     Suspended (if possible) or Stopped (if not).
        ///     Any Suspend exceptions are logged but ignored.
        virtual ~VirtualMachine();

        //////////
        //  Operations (general)
    public:
        /// \brief
        ///     Returns the name of this VM.
        /// \details
        ///     Can be safely called from any thread.
        /// \return
        ///     The name of this VM.
        QString         name() const;

        /// \brief
        ///     Sets the name of this VM.
        /// \details
        ///     Can be safely called from any thread.
        /// \param name
        ///     The new name of this VM.
        void            setName(const QString & name);

        /// \brief
        ///     Checks if the specified name is a valid VM name.
        /// \param name
        ///     The name to check.
        /// \return
        ///     True if the specified name is a valid VM name, else false.
        static bool     isValidName(const QString & name);

        /// \brief
        ///     Returns the location of this VM.
        /// \details
        ///     Can be safely called from any thread.
        /// \return
        ///     The full path to the configuration file of this VM.
        QString         location() const;

        /// \brief
        ///     Returns the full path to the "saved state" file
        ///     of this VM.
        /// \details
        ///     This file is always located alongsidde the VM
        ///     configuration file.
        /// \return
        ///     Returns the full path to the "saved state" file
        ///     of this VM.
        QString         savedStateFileName() const;

        /// \brief
        ///     Returns the Architecture of this VM.
        /// \details
        ///     Can be safely called from any thread.
        /// \return
        ///     The Architecture of this VM.
        IArchitecture * architecture() const;

        /// \brief
        ///     Returns the virtualMachineType of this VM.
        /// \details
        ///     Can be safely called from any thread.
        /// \return
        ///     The virtualMachineType of this VM.
        auto            type() const -> IVirtualMachineType *;

        /// \brief
        ///     Returns the Template from which this VM was created.
        /// \details
        ///     Can be safely called from any thread.
        /// \return
        ///     The Template from which this VM was created, nullptr == none.
        auto            createdFrom() const -> IVirtualMachineTemplate *;

        /// \brief
        ///     Returns the small (16x16) icon reprsenting this virtual machine.
        /// \return
        ///     The small (16x16) icon reprsenting this virtual machine.
        QIcon           smallIcon() const;

        /// \brief
        ///     Returns the large (32x32) icon reprsenting this virtual machine.
        /// \return
        ///     The large (32x32) icon reprsenting this virtual machine.
        QIcon           largeIcon() const;

        /// \brief
        ///     Checks whether this VM can save its runtime
        ///     state and reload it later on.
        /// \details
        ///     Can be safely called from any thread.
        /// \return
        ///     True if this VM can save its runtime state
        ///     and reload it later on, false if not.
        bool            isPersistable() const;

        //////////
        //  Operations (configuration)
    public:
        /// \brief
        ///     Checks whether the current configuration of this
        ///     VM is valid.
        /// \details
        ///     A VM will not permit setting invalid configuration
        ///     properties via confoguration-affecting methods; however,
        ///     that's no defense against a corrupt VM configuration
        ///     file which has been manually altered.
        /// \return
        ///     True the current configuration of this VM
        ///     is valid, else false.
        virtual bool    isConfigurationValid() const;

        /// \brief
        ///     Returns the set of all Components of this VM.
        /// \return
        ///     The set of all Components of this VM.
        Components      components() const;

        /// \brief
        ///     Returns the set of all native Components of this VM.
        /// \details
        ///     These are VM components for which adaptors did not need
        ///     to be created when adding them to this VM.
        /// \return
        ///     The set of all native Components of this VM.
        Components      nativeComponents() const;

        /// \brief
        ///     Returns the set of all adapted Components of this VM.
        /// \details
        ///     These are VM components for which adaptors had to be
        ///     created when adding them to this VM.
        /// \return
        ///     The set of all adapted Components of this VM.
        Components      adaptedComponents() const;

        /// \brief
        ///     Finds the ComponentAdaptor for the specified Component.
        /// \return
        ///     If the specified Compoonent a) belongs to (is owned by) this
        ///     VN and b) is adapted t it, returns the ComponentAdaptor that
        ///     wraps this component; otherwise returns nullptr.
        auto            findAdaptor(IComponent * component) const -> IComponentAdaptor *;

        /// \brief
        ///     Finds all components that are of the specified type.
        /// \return
        ///     The set of all VM components of the specified type.
        template <class T>
        auto            componentsOfType() const -> QSet<T*>;

        /// \brief
        ///     Adds an unbound Component to this VM, creating
        ///     a corresponding ComponentAdaptor if necessary.
        /// \details
        ///     -   If the Component is already bound to
        ///         this VM, the call has no effect.
        ///     -   If the Component is already bound to
        ///         another VM, the call is an error.
        ///     -   Of the Component is not compatible with or
        ///         adaptable to this VM, the call is an error.
        ///     -   Can be safely called from any thread.
        /// \param component
        ///     The Component to add to this VM.
        /// \param componentAdaptorType
        ///     If the component can be assed to this VM as a native
        ///     component, this parameter is ignored.
        ///     If the component requires adaptation to this VM, specifies
        ///     the preferred component adaptor type; if nullptr or
        ///     unavailable then any compatible component adaptor
        ///     type will be chosen automatically.
        /// \exception VirtualMachineException
        ///     If an error occurs (e.g. component is incompatible
        ///     with this VM or already belongs to another VM).
        void            addComponent(
                                IComponent * component,
                                IComponentAdaptorType * componentAdaptorType = nullptr
                            );

        /// \brief
        ///     Removes a bound Component from this VM, also
        ///     destroying the corresponding ComponentAdaptor
        ///     if one exists.
        /// \details
        ///     The Component is unbound after the call.
        ///     Can be safely called from any thread.
        /// \param component
        ///     The Component to remove from this VM.
        /// \exception VirtualMachineException
        ///     If an error occurs (e.g. component does
        ///     not belong to this VM).
        void            removeComponent(IComponent * component);

        QString         toAbsolutePath(const QString & relativePath);
        QString         toRelatimePath(const QString & absolutePath);

        //////////
        //  Operations (state control)
    public:
        /// \brief
        ///     Returns the State of this VM.
        /// \details
        ///     Can be safely called from any thread.
        /// \return
        ///     The State of this VM.
        State           state() const;

        bool            isStopped() const;
        bool            isRunning() const;
        bool            isSuspended() const;

        /// \brief
        ///     Perorms the Stopped -> Running or Suspended -> Running
        ///     state change.
        /// \details
        ///     -   All Stopped VM Components are connected, initialized
        ///         and started, in that order.
        ///     -   If the VM is not Stopped, the call is an error.
        ///     -   This method can be safely called from any thread.
        /// \exception VirtualMachineException
        ///     If an error occurs; the VM and component states remain
        ///     unchanged when an exception is thrown.
        void            start();

        /// \brief
        ///     Performs Running -> Stopped or Suspended -> Stopped
        ///     state change.
        ///     If the VM is already Stopped, the call has no effect.
        /// \details
        ///     -   All VM Components are stopped, deinitialized and
        ///         disconnected, in that order.
        ///     -   This method can be safely called from any thread.
        ///     -   If the VM is Suspended, the saved VM state is
        ///         destroyed by the call.
        void            stop() noexcept;

        /// \brief
        ///     Performs the Running -> Suspended state change.
        /// \details
        ///     -   All VM Components are stopped, then VM runtime
        ///         state is saved, then components are deinitialized
        ///         and disconnected.
        ///     -   If one of the VM Components does not support
        ///         persistency, the call is an error.
        ///     -   This method can be safely called from any thread.
        /// \exception VirtualMachineException
        ///     If an error occurs; the AM and component states remain
        ///     unchanged when an exception is thrown.
        void            suspend();

        /// \brief
        ///     Performs the Suspended -> Running state change.
        /// \details
        ///     -   All VM Components are connected and initialized,
        ///         then their runtime state is loaded, then all
        ///         Components are started.
        ///     -   If one of the VM Components does not support
        ///         persistency, the call is an error.
        ///     -   If the VM is not Suspended, the call has no effect.
        ///     -   This method can be safely called from any thread.
        /// \exception VirtualMachineException
        ///     If an error occurs.
        void            resume();

        /// \brief
        ///     Performs a soft reset of a Running VM.
        /// \details
        ///     If this VM is not Running, the call has no effect.
        /// \exception VirtualMachineException
        ///     If an error occurs.
        void            reset();

        /// \brief
        ///     Performs a restart (hard reset) of a Running VM.
        /// \details
        ///     If this VM is not Running, the call has no effect.
        /// \exception VirtualMachineException
        ///     If an error occurs.
        void            restart();

    signals:
        /// \brief
        ///     Emitted when the State of this VM changes.
        /// \param virtualMachine
        ///     The VM whose state has changed.
        void            stateChanged(VirtualMachine * virtualMachine);

        /// \brief
        ///     Emitted when the VM itself expresses a wish
        ///     to be stopped.
        /// \param virtualMachine
        ///     The VM making the wish.
        void            stopRequired(VirtualMachine * virtualMachine);

        /// \brief
        ///     Emitted when the VM itself expresses a wish
        ///     to be restarted (hard reset).
        /// \param virtualMachine
        ///     The VM making the wish.
        void            restartRequired(VirtualMachine * virtualMachine);

        /// \brief
        ///     Emitted when the VM itself expresses a wish
        ///     to perform a soft reset.
        /// \param virtualMachine
        ///     The VM making the wish.
        void            resetRequired(VirtualMachine * virtualMachine);

        //////////
        //  Operations (persistency)
    public:
        /// \brief
        ///     Saves the VM definition to its "location".
        /// \exception VirtualMachineException
        ///     If an error occurs.
        void            save();

        /// \brief
        ///     Loads a VM at the specified location.
        /// \param location
        ///     The path to the VM configuration file.
        /// \return
        ///     The loaded VM (in Stopped or Suspended state).
        /// \exception VirtualMachineException
        ///     If an error occurs.
        static auto     load(const QString & location) -> VirtualMachine *;

        //////////
        //  Implementation
    private:
        State           _state = State::Stopped;
        mutable emuone::util::Mutex _guard; //  for all state changes

        QString         _name;
        const QString   _location;  //  always full path
        IArchitecture *const            _architecture;  //  never nullptr
        IVirtualMachineType *const      _type;          //  never nullptr
        IVirtualMachineTemplate *const  _createdFrom;   //  may be nullptr

        Components          _nativeComponents;
        Components          _adaptedComponents;
        ComponentAdaptors   _adaptors;  //  for all _adaptedComponents

        //  Helpers
        void                _connectComponents();
        void                _initializeComponents();
        void                _startCompoonents();
        void                _stopCompoonents();
        void                _deinitializeComponents();
        void                _disconnectComponents();
    };

    template <class T>
    auto VirtualMachine::componentsOfType() const -> QSet<T*>
    {
        emuone::util::Lock _(_guard);

        QSet<T*> result;
        for (auto c : components())
        {
            if (auto t = dynamic_cast<T*>(c))
            {   //  Match!
                result.insert(t);
            }
        }
        return result;
    }
}

//  End of emuone-core/VirtualMachine.hpp
