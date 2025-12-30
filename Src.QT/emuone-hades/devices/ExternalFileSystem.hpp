//
//  emuone-hades/devices/ExternalFileSystem.hpp - The external file system device
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
#include "emuone-hades/API.hpp"

namespace emuone::hades::devices
{
    /// \class ExternalFileSystem emuone-hades/API.hpp
    /// \brief The HADES external file system device.
    class EMUONE_HADES_PUBLIC ExternalFileSystem final
        :   public virtual emuone::core::IDevice
    {
        //////////
        //  Types
    public:
        class EMUONE_HADES_PUBLIC Type final
            :   public virtual emuone::core::IComponentType
        {
            EMUONE_DECLARE_SINGLETON(Type)

            //////////
            //  emuone::util::IStockObject
        public:
            virtual QString mnemonic() const override;
            virtual QString displayName() const override;

            //////////
            //  emuone::core::IComponentType
        public:
            virtual auto    category() const -> emuone::core::IComponentCategory * override;
            virtual bool    isCompatibleWith(emuone::core::IArchitecture * architecture) const override;
            virtual bool    isCompatibleWith(emuone::core::IVirtualMachineType * virtualMachineType) const override;
            virtual bool    isPersistable() const override;
            virtual auto    createComponent() -> ExternalFileSystem * override;
        };

        //////////
        //  Construction/destruction
    public:
        ExternalFileSystem();
        virtual ~ExternalFileSystem();

        //////////
        //  emuone::core::IComponent
    public:
        virtual auto    type() const -> emuone::core::IComponentType * override;
        virtual QString displayName() const override;
        virtual auto    createEditor(QWidget * parent) -> emuone::core::ComponentEditor * override;
        virtual void    saveConfiguration(QDomElement & element) const override;
        virtual void    restoreConfiguration(const QDomElement & element) override;

        //////////
        //  emuone::core::IComponent (state control)
    public:
        virtual State   state() const override;
        virtual void    connect() override;
        virtual void    disconnect() noexcept override;
        virtual void    initialize() override;
        virtual void    deinitialize() noexcept override;
        virtual void    start() override;
        virtual void    stop() noexcept override;

        //////////
        //  Operations (configuration)
    public:
        static bool     isValidVolumeName(const QString & volumeName);
        static bool     isValidHostPath(const QString & hostPath);

        QString         volumeName() const;
        void            setVolumeName(const QString & volumeName);
        QString         hostPath() const;
        void            setHostPath(const QString & hostPath);

        //////////
        //  Implementation
    private:
        State           _state = State::Constructed;

        //  Configuration
        QString         _volumeName;
        QString         _hostPath;
    };

    namespace Ui { class ExternalFileSystemEditor; }

    /// \class ExternalFileSystemEditor emuone-hades/API.hpp
    /// \brief The editor for external file system device.
    class EMUONE_HADES_PUBLIC ExternalFileSystemEditor final
        :   public emuone::core::ComponentEditor
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ExternalFileSystemEditor)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the editor.
        /// \param parent
        ///     The parent for the editor; nullptr == none.
        /// \param externalFileSystem
        ///     The External File System to edit.
        ExternalFileSystemEditor(
                QWidget * parent,
                ExternalFileSystem * externalFileSystem
            );

        /// \brief
        ///     The class destructor.
        virtual ~ExternalFileSystemEditor();

        //////////
        //  emuone::core::ComponentEditor
    public:
        virtual void    loadControlValues() override;
        virtual void    saveControlValues() const override;
        virtual bool    isValid() const override;

        //////////
        //  Implementation
    private:
        ExternalFileSystem *const   _externalFileSystem;

        //////////
        //  Controls
    private:
        Ui::ExternalFileSystemEditor *const _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _volumeNameLineEditTextChanged(QString);
        void            _hostPathLineEditTextChanged(QString);
        void            _browsePushButtonClicked();
    };
}

//  End of emuone-hades/devices/ExternalFileSystem.hpp
