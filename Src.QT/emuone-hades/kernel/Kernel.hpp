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
    /// \class Kernel emuone-hades/API.hpp
    /// \brief The HADES OS Kernel.
    class EMUONE_HADES_PUBLIC Kernel final
        :   public virtual emuone::core::IComponent
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Kernel)

        friend class Object;
        friend class SystemIdentity;

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

        //////////
        //  Operations (identity management)
    public:
        SystemIdentity *createSystemIdentity();

        //////////
        //  Implementation
    private:
        State           _state = State::Constructed;

        //  Configuration
        QVersionNumber  _version = DefaultVersion;

        //////////
        //  Runtime state
    public:
        /// \brief
        ///     Locked whenever running kernel code.
        /// \details
        ///     This guard functions as an "interrupts disabled"
        ///     hardware flag in real processors.
        emuone::util::Mutex kernelGuard;

    private:
        //  If a process is asked to terminate but does not
        //  do so within nthe specified time, it is force-killed
        static const int _GraceBeforeKillMs = 10000;

        //  The primary objact table - contains all live
        //  kernel objects and counts as a "reference"
        QMap<Oid, Object*>  _objects;

        //  Secondary object caches for access speedup -
        //  all of them do NOT count as "references"
        SystemIdentity *    _systemIdentity = nullptr;
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
        virtual bool    isValid() const override;

        //////////
        //  Implementation
    private:
        Kernel *const   _kernel;
        bool            _constructed = false;

        //
        void            _loadControlValues();
        void            _saveControlValues() const;

        //////////
        //  Controls
    private:
        Ui::KernelEditor *const _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _kernelVersionLineEditTextChanged(QString);
    };
}

//  Macro required to allow MOC compiler to do its work
#define EMUONE_HADED_SHARED_FOLDER_DEFINED

//  End of emuone-hades/kernel/Kernel.hpp
