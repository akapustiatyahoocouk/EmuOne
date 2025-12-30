//
//  emuone-hades/kernel/Kernel.hpp - The HADES OS Kernel
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

namespace emuone::hades::kernel
{
    /// \class SharedFolder emuoe-hades/API.hpp
    /// \brief A definition of a host folder that appears as
    ///        an external file system in a HADES kernel.
    class EMUONE_HADES_PUBLIC SharedFolder final
    {
        //////////
        //  Construction/destruction/assignment
    public:
        SharedFolder() = default;
        SharedFolder(const QString & volumeName, const QString & hostPath);

        //////////
        //  Operators
    public:
        bool        operator == (const SharedFolder & op2) const;
        bool        operator != (const SharedFolder & op2) const;
        bool        operator <  (const SharedFolder & op2) const;
        bool        operator <= (const SharedFolder & op2) const;
        bool        operator >  (const SharedFolder & op2) const;
        bool        operator >= (const SharedFolder & op2) const;

        //////////
        //  Operations
    public:
        bool        isValid() const;
        QString     volumeName() const { return _volumeName; }
        QString     hostPath() const { return _hostPath; }

        static bool isValidVolumeName(const QString & volumeName);
        static bool isValidHostPath(const QString & hostPath);

        //////////
        //  Implementation
    private:
        QString     _volumeName;    //  as known to HADES OS
        QString     _hostPath;      //  where data resides
    };
    using SharedFolders = QSet<SharedFolder>;

    inline size_t qHash(const SharedFolder & key, size_t seed)
    {
        return qHash(key.volumeName(), seed);
    }

    /// \class Kernel emuone-hades/API.hpp
    /// \brief The HADES OS Kernel.
    class EMUONE_HADES_PUBLIC Kernel final
        :   public virtual emuone::core::IComponent
    {
        //////////
        //  Constants
    public:
        /// \brief
        ///     The default kernel version number.
        inline static const QVersionNumber DefaultVersion{1, 0, 0};

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
            virtual auto    createComponent() -> Kernel * override;
        };

        //////////
        //  Construction/destruction
    public:
        Kernel();
        virtual ~Kernel();

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
        QVersionNumber  version() const;
        void            setVersion(const QVersionNumber & version);
        SharedFolders   sharedFolders() const;
        void            setSharedFolders(const SharedFolders & sharedFolders);

        //////////
        //  Implementation
    private:
        State           _state = State::Constructed;

        //  Configuration
        QVersionNumber  _version = DefaultVersion;
        SharedFolders   _sharedFolders; //  All VolumeNames different!!!
    };

    namespace Ui { class KernelEditor; }

    /// \class KernelEditor emuone-hades/API.hpp
    /// \brief The editor for Kernel components.
    class EMUONE_HADES_PUBLIC KernelEditor final
        :   public emuone::core::ComponentEditor
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(KernelEditor)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the editor.
        /// \param parent
        ///     The parent for the editor; nullptr == none.
        /// \param kernel
        ///     The Kernel to edit.
        KernelEditor(QWidget * parent, Kernel * kernel);

        /// \brief
        ///     The class destructor.
        virtual ~KernelEditor();

        //////////
        //  emuone::core::ComponentEditor
    public:
        virtual void    loadControlValues() override;
        virtual void    saveControlValues() const override;
        virtual bool    isValid() const override;

        //////////
        //  Implementation
    private:
        Kernel *const   _kernel;
        inline static const QString _SharedFoldersSeparator = " -> ";

        //  Helpers
        void            _refresh();
        SharedFolder    _selectedSharedFolder() const;
        SharedFolders   _sharedFolders() const;
        void            _setSharedFolders(const SharedFolders & sharedFolders);
        void            _addSharedFolder(const SharedFolder & sharedFolder);
        void            _removeSharedFolder(const QString & volumeName);

        //////////
        //  Controls
    private:
        Ui::KernelEditor *const _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _kernelVersionLineEditTextChanged(QString);
        void            _sharedFoldersListWidgetCurrentRowChanged(int);
        void            _addSharedFolderPushButtonClicked();
        void            _modifySharedFolderPushButtonClicked();
        void            _removeSharedFolderPushButtonClicked();
    };
}

//////////
//  Formatting/parwing
namespace emuone::util
{
    template <> EMUONE_HADES_PUBLIC
    QString toString<emuone::hades::kernel::SharedFolder>(const emuone::hades::kernel::SharedFolder & value);

    template <> EMUONE_HADES_PUBLIC
    QString toString<emuone::hades::kernel::SharedFolders>(const emuone::hades::kernel::SharedFolders & value);

    template <> EMUONE_HADES_PUBLIC
    emuone::hades::kernel::SharedFolder
    fromString<emuone::hades::kernel::SharedFolder>(const QString & s, qsizetype & scan);

    template <> EMUONE_HADES_PUBLIC
    emuone::hades::kernel::SharedFolders
    fromString<emuone::hades::kernel::SharedFolders>(const QString & s, qsizetype & scan);
}

//  Macro required to allow MOC compiler to do its work
#define EMUONE_HADED_SHARED_FOLDER_DEFINED

//  End of emuone-hades/kernel/Kernel.hpp
