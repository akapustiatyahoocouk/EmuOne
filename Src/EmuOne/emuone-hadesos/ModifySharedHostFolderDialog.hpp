//
//  emuone-hadesos/ModifySharedHostFolderDialog.hpp
//
//  Hades OS "unit editor modify shared host folder" dialog
//
//////////
#pragma once
#include "emuone-hadesos/API.hpp"
#include <QWidget>

namespace hadesos
{

    namespace Ui { class ModifySharedHostFolderDialog; }

    //////////
    //  Hades OS "unit editor modify shared host folder" dialog
    class ModifySharedHostFolderDialog : public QDialog
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ModifySharedHostFolderDialog)

        //////////
        //  Construction/destruction
    public:
        explicit ModifySharedHostFolderDialog(const HadesOs::SharedHostFolder & sharedHostFolder, QWidget * parent = nullptr);
        ~ModifySharedHostFolderDialog();

        //////////
        //  Operations
    public:
        QString             sharedHostFolderVolumeName() const { return _sharedHostFolderVolumeName; }
        QString             sharedHostFolderHostPath() const { return _sharedHostFolderHostPath; }

        //////////
        //  Implementation
    private:
        Ui::ModifySharedHostFolderDialog *  _ui;
        HadesOs::SharedHostFolder   _originalSharedHostFolder;
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

//  End of emuone-hadesos/ModifySharedHostFolderDialog.hpp
