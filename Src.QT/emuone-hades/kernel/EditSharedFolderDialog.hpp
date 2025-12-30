//
//  emuone-hades/kernel/EditSharedFolderDialog.hpp - The ,odal  "edit shared folder" dialog
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

#ifdef EMUONE_HADED_SHARED_FOLDER_DEFINED

namespace emuone::hades::kernel
{
    namespace Ui { class EditSharedFolderDialog; }

    /// \class SharedFolder emuoe-hades/API.hpp
    /// \brief A definition of a host folder that appears as
    ///        an external file system in a HADES kernel.
    class EMUONE_HADES_PUBLIC EditSharedFolderDialog final
        :   private QDialog
    {
        Q_OBJECT
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(EditSharedFolderDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< User's choice confirmed.
            Cancel  ///< User has cancelled the dialog.
        };

        //////////
        //  Construction/destructoion
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent for the dlaigd; nullptr == none,
        /// \param sharedFolder
        ///     The shared folder to start editing from.
        explicit EditSharedFolderDialog(
                QWidget * parent,
                const SharedFolder & sharedFolder = SharedFolder()
            );

        /// \brief
        ///     The class destructor.
        virtual ~EditSharedFolderDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Invoks the dialog modally.
        /// \return
        ///     The dialog result.
        Result          doModal();

        /// \brief
        ///     Returns the shared folder as specified by the user.
        /// \return
        ///     The shared folder as specified by the user.
        SharedFolder    sharedFolder() const { return _sharedFolder; }

        //////////
        //  Implementation
    private:
        SharedFolder    _sharedFolder;

        //  Helpers
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::EditSharedFolderDialog *const   _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _volumeNameLineEditTextChanged(QString);
        void            _hostPathLineEditTextChanged(QString);
        void            _browsePushButtonClicked();
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

#endif  //  def EMUONE_HADED_SHARED_FOLDER_DEFINED
//  End of emuone-hades/kernel/EditSharedFolderDialog.hpp
