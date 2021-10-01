//
//  emuone/NewVmDialog.cpp
//
//  "Main UI frame"New VM" dialog
//
//////////
#include "emuone/API.hpp"

#include "NewVmDialog.hpp"
#include "ui_NewVmDialog.h"

NewVmDialog::NewVmDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewVmDialog)
{
    ui->setupUi(this);

    //  Populate "architectures" combo box
    ArchitectureList architectures = Architecture::getAll();
    if (architectures.isEmpty())
    {   //  No architectures - can't do anything really
        ui->_templateLabel->setEnabled(false);
        ui->_templateComboBox->setEnabled(false);
        ui->_nameLabel->setEnabled(false);
        ui->_nameLineEdit->setEnabled(false);
        ui->_locationLabel->setEnabled(false);
        ui->_locationLineEdit->setEnabled(false);
        ui->_browseButton->setEnabled(false);
    }
    else
    {   //  SOME architectures are defined
        for (Architecture * architecture : architectures)
        {
            ui->_architectureComboBox->addItem(architecture->getSmallIcon(), architecture->getDisplayName(), QVariant::fromValue(architecture));
        }
        ui->_architectureComboBox->setCurrentIndex(0);
    }

    //  Done
    _refresh();
}

NewVmDialog::~NewVmDialog()
{
    delete ui;
}

//////////
//  Implementation helpers
void NewVmDialog::_refresh()
{
    ui->_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(_getSelectedArchitecture() != nullptr);
}

Architecture * NewVmDialog::_getSelectedArchitecture()
{
    int n = ui->_architectureComboBox->currentIndex();
    if (n >= 0)
    {
        return (Architecture*)(ui->_architectureComboBox->itemData(n).value<void*>());
    }
    return nullptr;
}

//  End of emuone/NewVmDialog.cpp
