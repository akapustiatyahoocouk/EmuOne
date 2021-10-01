//
//  emuone/NewVmDialog.hpp
//
//  "New VM" dialog
//
//////////
#pragma once
#include "emuone/API.hpp"
#include <QDialog>

namespace Ui {
class NewVmDialog;
}

//////////
//  "New VM" dialog
class NewVmDialog : public QDialog
{
    Q_OBJECT

    //////////
    //  Construction/destruction
public:
    explicit NewVmDialog(QWidget *parent = nullptr);
    ~NewVmDialog();

    //////////
    //  Implementation
private:
    Ui::NewVmDialog *ui;

    //  Helpers
    void                _refresh();
    Architecture *      _getSelectedArchitecture();
};

//  End of emuone/NewVmDialog.hpp
