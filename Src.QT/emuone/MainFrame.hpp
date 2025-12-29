//
//  emuone/MainFrame.hpp - Main UI frame
//
//  TimeTracker3
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
#include "emuone/API.hpp"

namespace emuone
{
    namespace Ui { class MainFrame; }

    /// \class MainFrame emuone/API.hpp
    /// \brief The main UI frame.
    class MainFrame final
        :   public QMainWindow
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MainFrame)

        //////////
        //  Constants
    public:
        /// \brief
        ///     The minimum permitted size for a main frame.
        inline static const QSize MinimumSize { 320, 200 };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs (but does not show) the main frame.
        MainFrame();

        /// \brief
        ///     The class destructor.
        virtual ~MainFrame();

        //////////
        //  QWidget
    public:
        /// \brief
        ///     Called to handle a "window move" event
        /// \param event
        ///     The event detils.
        virtual void    moveEvent(QMoveEvent * event) override;

        /// \brief
        ///     Called to handle a "window resize" event
        /// \param event
        ///     The event detils.
        virtual void    resizeEvent(QResizeEvent * event) override;

        /// \brief
        ///     Called to handle a "window close" event
        /// \param event
        ///     The event detils.
        virtual void	closeEvent(QCloseEvent * event) override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Shows this frame; has no effect if already visible.
        void            show();

        /// \brief
        ///     Hides this frame; has no effect if already invisible.
        void            hide();

        /// \brief
        ///     Returns the list of all currently open VMs.
        /// \return
        ///     The list of all currently open VMs.
        auto            virtualMachines(
                            ) const -> emuone::core::VirtualMachines;

        /// \brief
        ///     Returns the VM that is "current" in this frame.
        /// \return
        ///     The VM that is "current" in this frame; nullptr == none (home page).
        auto            currentVirtualMachine(
                            ) const -> emuone::core::VirtualMachine *;

        /// \brief
        ///     Sets the VM that is "current" in this frame.
        /// \param virtualMachine
        ///     The VM to make "current" in this frame; nullptr == none (home page).
        void            setCurrentVirtualMachine(
                                emuone::core::VirtualMachine * virtualMachine
                            );

        /// \brief
        ///     Refreshes this main frame and controls within
        void            refresh();

        //////////
        //  Implementation
    private:
        using VirtualMachines = emuone::core::VirtualMachines;

        bool            _trackPosition = false;

        //  Helpers
        void            _loadPosition();
        void            _savePosition();

        void            _refreshMruList();

        auto            _openVirtualMachine(
                                const QString & location
                            ) -> emuone::core::VirtualMachine *;  //  may throw

        //////////
        //  Controls
    private:
        Ui::MainFrame *const    _ui;
        QTimer                  _trackPositionTimer;
        QTimer                  _savePositionTimer;
        QTimer                  _refreshTimer;

        //////////
        //  Signal handlers
    private slots:
        void            _trackPositionTimerTimeout();
        void            _savePositionTimerTimeout();
        void            _refreshTimerTimeout();

        void            _onActionNewVirtualMachine();
        void            _onActionOpenVirtualMachine();
        void            _onActionExit();
    };
}

//  End of  emuone/MainFrame.hpp
