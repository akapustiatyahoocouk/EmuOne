//
//  emuone-util/Dynaload.hpp - Dynamic loading support
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

namespace emuone::util
{
    /// \class IComponent emuone-util/API.hpp
    /// \brief A generic component making up the EmuOne.
    class EMUONE_UTIL_PUBLIC IComponent
        :   public virtual IStockObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IComponent)

        //////////
        //  Construction/destruction
    protected:
        IComponent() = default;
        virtual ~IComponent() = default;

        //////////
        //  IStockObject
    public:
        virtual QImage  smallImage() const override;
        virtual QImage  largeImage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the version number of this stock object.
        /// \return
        ///     The version number of this stock object.
        virtual auto    version() const -> QVersionNumber = 0;

        /// \brief
        ///     Returns the user-readable copyright message of this component.
        /// \return
        ///     The user-readable copyright message of this component.
        virtual QString copyright() const = 0;

        /// \brief
        ///     Returns this Component's Settings.
        /// \return
        ///     This Component's Settings.
        virtual auto    settings() -> Settings * = 0;

        /// \brief
        ///     Returns this Component's Settings.
        /// \return
        ///     This Component's Settings.
        virtual auto    settings() const -> const Settings * = 0;

        /// \brief
        ///     Initializes this component; has no effect if
        ///     the component has already been initialized.
        /// \exception Exception
        ///     If the component initialization fails.
        virtual void    iniialize() = 0;

        /// \brief
        ///     Deinitializes this component; has no effect if
        ///     the component has not been initialized.
        virtual void    deiniialize() = 0;
    };

    /// \class ComponentManager emuone-util/API.hpp
    /// \brief The manager of known components.
    class EMUONE_UTIL_PUBLIC ComponentManager final
    {
        EMUONE_UTILITY_CLASS(ComponentManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered components.
        /// \return
        ///     Returns the set of all registered components.
        static auto     components() -> Components;

        /// \brief
        ///     Registers the specified component.
        /// \details
        ///     Registering an already-registered component
        ///     does nothing and returns true (success).
        /// \param component
        ///     The component to register.
        /// \return
        ///     True on success, false on failure.
        static bool     registerComponent(IComponent *cComponent);

        /// \brief
        ///     Un-registers the specified component.
        /// \param component
        ///     The component to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregisterComponent(IComponent * component);

        /// \brief
        ///     Finds a registered component by mnemonic and version.
        /// \param mnemonic
        ///     The component mnemonic to look for.
        /// \param version
        ///     The component version to look for.
        /// \return
        ///     The registered component with the required mnemonic
        ///     and version or nullptr if not found.
        static auto     findComponent(
                                const QString & mnemonic,
                                const QVersionNumber & version
                            ) -> IComponent *;

        /// \brief
        ///     Finds a registered component by mnemonic.
        /// \details
        ///     If several versions of component with the same
        ///     mnemonic are registered, finds the one with the
        ///     latest version.
        /// \param mnemonic
        ///     The component mnemonic to look for.
        /// \return
        ///     The latest available version of a registered Component
        ///     with the required mnemonic or nullptr if none found.
        static auto     findComponent(const QString & mnemonic) -> IComponent *;

        /// \brief
        ///     Loads (bit does NOT initialize) optional Components
        ///     available in an EmuOne installation.
        /// \details
        ///     This is done by loading those .DLL/.SO files in the
        ///     directory where the EmuOne executable was launched from
        ///     which define Components.
        static void     discoverComponents();

        /// \brief
        ///     Initializes all known Components by calling
        ///     initialize() on each of them.
        /// \details
        ///     Any exceptions thrown by Component initializers
        ///     are logged, but do not stop the initialization
        ///     process. This was as many Cmponents as possible
        ///     are initialized.
        static void     initializeComponents();

        /// \brief
        ///     Deinitializes all known Components by calling
        ///     deinitialize() on each of them.
        static void     deinitializeComponents();

        /// \brief
        ///     Loads Settings of all registered components from
        ///     an application-wide text configuration file.
        /// \details
        ///     This file is a hidden file located in the user's
        ///     home directory.
        ///     Any settings not explicitly present in the configuration
        ///     file retain their default values.
        static void     loadComponentSettings();

        /// \brief
        ///     Saves Settings of all registered components to
        ///     an application-wide text configuration file.
        /// \details
        ///     This file is a hidden file located in the user's
        ///     home directory.
        static void     saveComponentSettings();

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
        static void     _loadLibrary(const QString & fileName);
    };
}

//  A helper macro for Component declaration - use within a .hpp
//  file in a declaration of an IComponent - implementing class
#define EMUONE_DECLARE_COMPONENT(Clazz) \
    EMUONE_DECLARE_SINGLETON(Clazz)     \
    public:                             \
        class Registrator final         \
        {                               \
            EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Registrator) \
            public:                     \
                Registrator();          \
                ~Registrator();         \
        };

//  A helper macro for Component definition - use
//  within a .cpp file where the IComponent -
//  implementing class is defined
#define EMUONE_IMPLEMENT_COMPONENT(Clazz)   \
    EMUONE_IMPLEMENT_SINGLETON(Clazz)       \
    Clazz::Clazz() {}                       \
    Clazz::~Clazz() {}                      \
    Clazz::Registrator::Registrator()       \
    {                                       \
        emuone::util::ComponentManager::registerComponent(Clazz::instance());   \
    }                                       \
    Clazz::Registrator::~Registrator()      \
    {                                       \
        emuone::util::ComponentManager::unregisterComponent(Clazz::instance()); \
    }                                       \
    namespace                               \
    {                                       \
        Clazz::Registrator the##Clazz##Registrator; \
    }

namespace emuone::util
{
    /// \class Component emuone-util/API.hpp
    /// \brief The emuone-util component.
    class EMUONE_UTIL_PUBLIC Component final
        :   public virtual IComponent
    {
        EMUONE_DECLARE_COMPONENT(Component)

        //////////
        //  Types
    public:
        /// \class Settings emuone-util/API.hpp
        /// \brief The component's settings.
        class Settings : public emuone::util::Settings
        {
            EMUONE_DECLARE_SINGLETON(Settings)
        };

        //////////
        //  IStockObject
    public:
        virtual QString mnemonic() const override;
        virtual QString displayName() const override;

        //////////
        //  IComponent
    public:
        virtual auto    version() const -> QVersionNumber override;
        virtual QString copyright() const override;
        virtual auto    settings() -> Settings * override;
        virtual auto    settings() const -> const Settings * override;
        virtual void    iniialize() override;
        virtual void    deiniialize() override;
    };
}

//  End of emuone-util/Dynaload.hpp
