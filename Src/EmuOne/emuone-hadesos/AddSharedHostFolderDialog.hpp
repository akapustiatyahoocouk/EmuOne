//
//  emuone-hadesos/AddSharedHostFolderDialog.hpp
//
//  Hades OS "unit editor"add shared host folder" dialog
//
//////////
#pragma once
#include "emuone-hadesos/API.hpp"
#include <QWidget>

namespace hadesos {

    namespace Ui { class AddSharedHostFolderDialog; }

    //////////
    //  Hades OS "unit editor"add shared host folder" dialog
    class EMUONE_HADESOS_EXPORT AddSharedHostFolderDialog : public QDialog
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AddSharedHostFolderDialog)

        //////////
        //  Construction/destruction
    public:
        explicit AddSharedHostFolderDialog(QWidget * parent = nullptr);
        ~AddSharedHostFolderDialog();

        //////////
        //  Operations
    public:
        QString             sharedHostFolderVolumeName() const { return _sharedHostFolderVolumeName; }
        QString             sharedHostFolderHostPath() const { return _sharedHostFolderHostPath; }

        //////////
        //  Implementation
    private:
        Ui::AddSharedHostFolderDialog * _ui;
        QString             _sharedHostFolderVolumeName = "";
        QString             _sharedHostFolderHostPath = "";

        //  Helpers
        void                _refreshActionButons();

        //////////
        //  Event handlers
    private slots:
        void                _volumeNameLineEditTextChanged(const QString &);
        void                _hostPathLineEditTextChanged(const QString &);
        void                _browsePushButtonClicked();
        void                _accept();
        void                _reject();
    };
}

//  End of emuone-hadesos/AddSharedHostFolderDialog.hpp
