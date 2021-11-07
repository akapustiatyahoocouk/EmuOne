//
//  emuone-scp360/ModifySharedFolderDialog.hpp
//
//  "Modify shared folder" modal dialog
//
//////////
#pragma once
#include "emuone-scp360/API.hpp"
#include <QDialog>

namespace scp360
{
    namespace Ui { class ModifySharedFolderDialog; }

    //////////
    //  "Modify shared folder" modal dialog
    class ModifySharedFolderDialog : public QDialog
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ModifySharedFolderDialog)

        //////////
        //  Construction/destruction
    public:
        explicit ModifySharedFolderDialog(Scp::SharedFolder * sharedFolder, QWidget *parent = nullptr);
        ~ModifySharedFolderDialog();

        //////////
        //  Implementation
    private:
        Ui::ModifySharedFolderDialog *  _ui;
        Scp::SharedFolder *             _sharedFolder;

        //////////
        //  Helpers
        void                _refresh();

        //////////
        //  Event handlers
    private slots:
        void                _browsePushButtonClicked();
        void                _accept();
        void                _reject();
    };
}
//  End of emuone-scp360/ModifySharedFolderDialog.hpp
