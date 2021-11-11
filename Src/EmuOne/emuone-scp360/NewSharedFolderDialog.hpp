//
//  emuone-scp360/NewSharedFolderDialog.hpp
//
//  "New shared folder" modal dialog
//
//////////
#pragma once
#include "emuone-scp360/API.hpp"
#include <QDialog>

namespace scp360
{
    namespace Ui { class NewSharedFolderDialog; }

    //////////
    //  "New shared folder" modal dialog
    class NewSharedFolderDialog final : public QDialog
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(NewSharedFolderDialog)

        //////////
        //  Construction/destruction
    public:
        explicit NewSharedFolderDialog(QWidget * parent = nullptr);
        virtual ~NewSharedFolderDialog();

        //////////
        //  Operations
    public:
        //  The volume name / host path selected by the user for a new shared folder.
        QString             volumeName() const { return _volumeName; }
        QString             hostPath() const { return _hostPath; }

        //////////
        //  Implementation
    private:
        Ui::NewSharedFolderDialog * _ui;

        QString             _volumeName = {};
        QString             _hostPath = {};

        //  Helpers
        void                _refresh();

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

//  End of emuone-scp360/NewSharedFolderDialog.hpp

