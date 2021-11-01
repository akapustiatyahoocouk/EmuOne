//
//  emuone-scp360/NewSharedFolderDialog.hpp
//
//  "New shared folder" modal dialog
//
//////////
#pragma once
#include "emuone-scp360/API.hpp"
#include <QDialog>

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
    //  Implementation
private:
    Ui::NewSharedFolderDialog * _ui;

    //////////
    //  Event handlers
private slots:
    void                _accept();
    void                _reject();
};

//  End of emuone-scp360/NewSharedFolderDialog.hpp

