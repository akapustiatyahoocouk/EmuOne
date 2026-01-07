//
//  emuone-hades/devicedrivers/DeviceDriver.hpp - HADES device driver framework
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

namespace emuone::hades::devicedrivers
{
    /// \class IDeviceDriver emuone-hades/API.hpp
    /// \brief A device driver used by HADES Kernel.
    class EMUONE_HADES_PUBLIC IDeviceDriver
        :   public virtual emuone::util::IStockObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IDeviceDriver)

        //////////
        //  This is an interface
    protected:
        IDeviceDriver() = default;
        virtual ~IDeviceDriver() = default;

        //////////
        //  emuone::util::IStockObject
    public:
        virtual QIcon   smallIcon() const override;
        virtual QIcon   largeIcon() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Creates a Runner for a NativeThread that
        ///     will act as a "device driver" for the
        ///     specified component.
        /// \param component
        ///     The component to create a device driver
        ///     runner for.
        /// \return
        ///     A newly created Runner for a NativeThread
        ///     that will act as a "device driver" for the
        ///     specified component.
        ///     If this DeviceDriver does not know how to
        ///     drive the component, returns nullptr.
        virtual auto    createRunner(
                                emuone::core::IComponent * component
                            ) -> kernel::NativeThreadRunner * = 0;

        /// \brief
        ///     Creates a Runner for a NativeThread that
        ///     will act as a "device driver" for the
        ///     specified component adaptor.
        /// \param componentAdaptor
        ///     The component adaptor to create a device driver
        ///     runner for.
        /// \return
        ///     A newly created Runner for a NativeThread
        ///     that will act as a "device driver" for the
        ///     specified component adaptor.
        ///     If this DeviceDriver does not know how to
        ///     drive the component adaptor, returns nullptr.
        virtual auto    createRunner(
                                emuone::core::IComponentAdaptor * componentAdaptor
                            ) -> kernel::NativeThreadRunner * = 0;
    };

    /// \class DeviceDriverManager emuone-hades/API.hpp
    /// \brief The manager of known HADES DeviceDrivers.
    class EMUONE_HADES_PUBLIC DeviceDriverManager final
    {
        EMUONE_UTILITY_CLASS(DeviceDriverManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered DeviceDrivers.
        /// \return
        ///     Returns the set of all registered DeviceDrivers.
        static auto     all() -> DeviceDrivers;

        /// \brief
        ///     Registers the specified DeviceDriver.
        /// \details
        ///     Registering an already-registered DeviceDriver
        ///     does nothing and returns true (success).
        /// \param deviceDriver
        ///     The DeviceDriver to register.
        /// \return
        ///     True on success, false on failure.
        static bool     register(IDeviceDriver * deviceDriver);

        /// \brief
        ///     Un-registers the specified DeviceDriver.
        /// \param deviceDriver
        ///     The DeviceDriver to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregister(IDeviceDriver * deviceDriver);

        /// \brief
        ///     Finds a registered component by mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered DeviceDriver with the
        ///     required mnemonic or nullptr if none found.
        static auto     find(const QString & mnemonic) -> IDeviceDriver *;

        //////////
        //  Implementation
    private:
        struct _Impl;
        static _Impl *  _impl();
    };
}

//  End of emuone-hades/devicedrivers/DeviceDriver.hpp
