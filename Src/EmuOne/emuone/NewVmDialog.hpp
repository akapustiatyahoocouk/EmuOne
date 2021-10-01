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
    void                _refreshTemplatesList();
    Architecture *      _getSelectedArchitecture();
    VirtualApplianceTemplate *  _getSelectedTemplate();

    //////////
    //  Event handflers
private slots:
    void                _architectureComboBoxCurrentIndexChanged(int index);
    void                _templateComboBoxCurrentIndexChanged(int index);
};

//  End of emuone/NewVmDialog.hpp
