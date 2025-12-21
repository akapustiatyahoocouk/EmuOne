//
//  emuone-core/VirtualMachine.cpp - A complete VM
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
    /// \class VirtualMachine emuone-core/API.hpp
    /// \brief The complete VM.
    class EMUONE_CORE_PUBLIC VirtualMachine final
    {
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
            ///     A saved VM runtime state may or may not exist.
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
    public:
        VirtualMachine(
                const QString & name,
                const QString & location,
                IArchitecture * architecture,
                IVirtualMachineType * virtualMachineType,
                ITemplate * vmTemplate = nullptr
            );
        ~VirtualMachine();

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
        IVirtualMachineType *   virtualMachineType() const;

        /// \brief
        ///     Returns the Template from which this VM was created.
        /// \details
        ///     Can be safely called from any thread.
        /// \return
        ///     The Template from which this VM was created, nullptr == none.
        ITemplate *     createdFrom() const;

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
        ///     Adds an unbound Component to this VM.
        /// \details
        ///     -   If the Component is already bound to
        ///         this VM, the call has no effect.
        ///     -   If the Component is already bound to
        ///         another VM, the call is an error.
        ///     -   Of the Cmponent is not compatible with or
        ///         adaptable to this VM, the call is an error.
        ///     -   Can be safely called from any thread.
        /// \param component
        ///     The Component to add.
        /// \exception VirtualMachineException
        ///     If an error occurs.
        void            addComponent(IComponent * component);

        /// \brief
        ///     Removes a bound Component from this M.
        /// \details
        ///     Can be safely called from any thread.
        /// \param component
        ///     The Component to remove.
        /// \exception VirtualMachineException
        ///     If an error occurs.
        void            removeComponent(IComponent * component);

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

        /// \brief
        ///     Perorms the Stopped -> Running or Suspended -> Running
        ///     state change.
        /// \details
        ///     -   All VM Components are connected, initialized and
        ///         started, in that order.
        ///     -   If the VM is Suspended (and saved VM state exists),
        ///         it is loaded after all components have been initialized
        ///         but before they are started.
        ///     -   If the VM is Running, the call has no effect.
        ///     -   This method can be safely called from any thread.
        /// \exception VirtualMachineException
        ///     If an error occurs; the component state remains
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
        ///         state is saved, them components are deinitialized
        ///         and disconnected.
        ///     -   If one of the VM Components does not support
        ///         persistency, the call is an error.
        ///     -   This method can be safely called from any thread.
        /// \exception VirtualMachineException
        ///     If an error occurs; the component state remains
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
        void            resume();

        //////////
        //  Implementation
    private:
        State           _state = State::Stopped;
        mutable emuone::util::Mutex _guard; //  for all state changes

        QString         _name;
        const QString   _location;  //  always full path
        IArchitecture *         _architecture;  //  never nullptr
        IVirtualMachineType *   _type;    //  never nullptr
        ITemplate *             _template;      //  may be nullptr
    };
}

//  End of emuone-core/VirtualMachine.cpp
