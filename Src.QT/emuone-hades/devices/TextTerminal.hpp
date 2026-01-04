//
//  emuone-hades/devices/TextTerminal.hpp - The text terminal
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
    /// \class TextTerminal emuone-hades/API.hpp
    /// \brief The HADES text terminal.
    class EMUONE_HADES_PUBLIC TextTerminal final
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
            virtual auto    createComponent() -> TextTerminal * override;
        };

        /// \class Command emuone-hades/API.hpp
        /// \brief A generic command accepted by an ExyFS device.
        class EMUONE_HADES_PUBLIC Command
            :   public IDevice::Command
        {
            EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Command)

            //////////
            //  Construction/destruction
        public:
            Command(uintmax_t sequenceNumber)
                :   IDevice::Command(sequenceNumber) {}
        };

        /// \class Response emuone-hades/API.hpp
        /// \brief A generic response from an ExyFS device.
        class EMUONE_HADES_PUBLIC Response
            :   public IDevice::Response
        {
            EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Response)

            //////////
            //  Construction/destruction
        public:
            Response(uintmax_t sequenceNumber)
                :   IDevice::Response(sequenceNumber) {}
        };

        //////////
        //  Construction/destruction
    public:
        TextTerminal();
        virtual ~TextTerminal();

        //////////
        //  emuone::core::IComponent
    public:
        virtual auto    type() const -> emuone::core::IComponentType * override;
        virtual QString displayName() const override;
        virtual auto    createEditor(QWidget * parent) -> emuone::core::ComponentEditor * override;

        //////////
        //  emuone::core::IComponent (configuration)
    public:
        virtual bool    isConfigurationValid() const override;
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
        //  emuone::core::IDevice
    public:
        virtual auto    sendCommand(
                IDevice::Command * command
            ) -> SendCommandOutcome override;

        //////////
        //  Operations (configuration)
    public:
        inline static const int MinTerminalNumber = 0;
        inline static const int MaxTerminalNumber = 255;
        inline static const int DefaultTerminalNumber = 0;

        inline static const int MinColumns = 40;
        inline static const int MaxColumns = 132;
        inline static const int DefaultColumns = 80;

        inline static const int MinRows = 4;
        inline static const int MaxRows = 60;
        inline static const int DefaultRows = 25;

        static bool     isValidTerminalNumber(int terminalNumber);
        static bool     isValidColumns(int columns);
        static bool     isValidRows(int rows);

        int             terminalNumber() const;
        void            setTerminalNumber(int terminalNumber);
        int             columns() const;
        void            setColumns(int columns);
        int             rows() const;
        void            setRows(int rows);

        //////////
        //  Implementation
    private:
        State           _state = State::Constructed;

        //  Configuration
        int             _terminalNumber = DefaultTerminalNumber;
        int             _columns = DefaultColumns;
        int             _rows = DefaultRows;

        //////////
        //  Threads
    private:
        class _NotificationThread : public QThread
        {
            EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_NotificationThread)

            //////////
            //  Constants
        public:
            static const int WaitChunkMs = 500;

            //////////
            //  Construction/destruction
        public:
            _NotificationThread(TextTerminal * extfs)
                :   _extfs(extfs) {}

            //////////
            //  QThread
        protected:
            virtual void    run() override;

            //////////
            //  Operations
        public:
            void        requestStop() { _stopRequested = true; }
            void        postResponse(Response * response)
            {
                Q_ASSERT(response != nullptr);
                _pendingResponses.enqueue(response);
            }

            //////////
            //  Implementation
        private:
            TextTerminal *const   _extfs;
            std::atomic<bool>   _stopRequested = false;
            emuone::util::BlockingQueue<Response*>  _pendingResponses;
        };
        _NotificationThread *   _notificationThread = nullptr;
    };

    namespace Ui { class TextTerminalEditor; }

    /// \class TextTerminalEditor emuone-hades/API.hpp
    /// \brief The editor for external file system device.
    class EMUONE_HADES_PUBLIC TextTerminalEditor final
        :   public emuone::core::ComponentEditor
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(TextTerminalEditor)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the editor.
        /// \param parent
        ///     The parent for the editor; nullptr == none.
        /// \param textTerminal
        ///     The External File System to edit.
        TextTerminalEditor(
                QWidget * parent,
                TextTerminal * textTerminal
            );

        /// \brief
        ///     The class destructor.
        virtual ~TextTerminalEditor();

        //////////
        //  emuone::core::ComponentEditor
    public:
        virtual bool    isValid() const override;

        //////////
        //  Implementation
    private:
        TextTerminal *const   _textTerminal;
        bool            _constructed = false;

        //  Helpers
        void            _loadControlValues();
        void            _saveControlValues() const;
        int             _selectedTerminalNumber() const;
        void            _setSelectedTerminalNumber(int terminalNumber);
        int             _selectedColumns() const;
        void            _setSelectedColumns(int columns);
        int             _selectedRows() const;
        void            _setSelectedRows(int rows);

        //////////
        //  Controls
    private:
        Ui::TextTerminalEditor *const _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _terminalNumberComboBoxCurrentIndexChanged(int);
        void            _columnsComboBoxCurrentIndexChanged(int);
        void            _rowsComboBoxCurrentIndexChanged(int);
    };
}

//  End of emuone-hades/devices/TextTerminal.hpp
