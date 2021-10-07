//
//  emuone/NewVmDialog.cpp
//
//  "New VM" dialog
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
    core::ArchitectureList architectures = core::Architecture::getAll();
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
        for (core::Architecture * architecture : architectures)
        {
            ui->_architectureComboBox->addItem(architecture->getSmallIcon(), architecture->getDisplayName(), QVariant::fromValue((void*)architecture));
        }
        ui->_architectureComboBox->setCurrentIndex(0);
    }

    //  Done
    _refreshTemplatesList();
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
    ui->_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(_getSelectedArchitecture() != nullptr &&
                                                             _getSelectedTemplate() != nullptr &&
                                                             core::VirtualAppliance::isValidName(ui->_nameLineEdit->text()) &&
                                                             !ui->_locationLineEdit->text().isEmpty());
}

void NewVmDialog::_refreshTemplatesList()
{
    core::VirtualApplianceTemplateList templateList;
    core::Architecture * architecture = _getSelectedArchitecture();
    if (architecture != nullptr)
    {
        if (ui->_virtualMachineRadioButton->isChecked())
        {
            for (core::VirtualMachineTemplate * virtualMachineTemplate : architecture->getVirtualMachineTemplates())
            {
                templateList.append(virtualMachineTemplate);
            }
        }
        else
        {
            for (core::RemoteTerminalTemplate * remoteTerminalTemplate : architecture->getRemoteTerminalTemplates())
            {
                templateList.append(remoteTerminalTemplate);
            }
        }
    }
    ui->_templateComboBox->clear();
    if (templateList.isEmpty())
    {   //  OOPS! No templates available!
        ui->_templateLabel->setEnabled(false);
        ui->_templateComboBox->setEnabled(false);
        ui->_nameLabel->setEnabled(false);
        ui->_nameLineEdit->setEnabled(false);
        ui->_locationLabel->setEnabled(false);
        ui->_locationLineEdit->setEnabled(false);
        ui->_browseButton->setEnabled(false);
    }
    else
    {   //  SOME templates available
        ui->_templateLabel->setEnabled(true);
        ui->_templateComboBox->setEnabled(true);
        ui->_nameLabel->setEnabled(true);
        ui->_nameLineEdit->setEnabled(true);
        ui->_locationLabel->setEnabled(true);
        ui->_locationLineEdit->setEnabled(true);
        ui->_browseButton->setEnabled(true);
        for (core::VirtualApplianceTemplate * virtualApplianceTemplate : templateList)
        {
            ui->_templateComboBox->addItem(virtualApplianceTemplate->getSmallIcon(),
                                           virtualApplianceTemplate->getDisplayName(),
                                           QVariant::fromValue((void*)virtualApplianceTemplate));
        }
        ui->_templateComboBox->setCurrentIndex(0);
    }
}

core::Architecture * NewVmDialog::_getSelectedArchitecture()
{
    int n = ui->_architectureComboBox->currentIndex();
    if (n >= 0)
    {
        return (core::Architecture*)(ui->_architectureComboBox->itemData(n).value<void*>());
    }
    return nullptr;
}

core::VirtualApplianceTemplate * NewVmDialog::_getSelectedTemplate()
{
    int n = ui->_templateComboBox->currentIndex();
    if (n >= 0)
    {
        return (core::VirtualApplianceTemplate*)(ui->_templateComboBox->itemData(n).value<void*>());
    }
    return nullptr;
}

//////////
//  Event handflers
void NewVmDialog::_architectureComboBoxCurrentIndexChanged(int /*index*/)
{
    _refreshTemplatesList();
    _refresh();
}

void NewVmDialog::_templateComboBoxCurrentIndexChanged(int /*index*/)
{
    _refresh();
}

void NewVmDialog::_nameLineEditTextChanged(const QString &)
{
    _refresh();
}

void NewVmDialog::_locationLineEditTextChanged(const QString &)
{
    _refresh();
}

void NewVmDialog::_browse()
{
    QString location = QFileDialog::getSaveFileName(this, "New VM location", "", "EmuOne VM file (*." + core::VirtualAppliance::PreferredExtension + ")");
    if (location.isEmpty())
    {   //  User has cancelled the dialog
        return;
    }
    QFileInfo fileInfo(location);
    if (fileInfo.suffix().isEmpty())
    {
        location += "." + core::VirtualAppliance::PreferredExtension;
    }
    ui->_locationLineEdit->setText(location);
}

void NewVmDialog::_accept()
{
    _virtualApplianceType =
            ui->_virtualMachineRadioButton->isChecked() ?
                (core::VirtualApplianceType*)core::VirtualMachine::Type::getInstance() :
                (core::VirtualApplianceType*)core::RemoteTerminal::Type::getInstance();
    _virtualApplianceArchitecture = _getSelectedArchitecture();
    _virtualApplianceTemplate = _getSelectedTemplate();
    _virtualApplianceName = ui->_nameLineEdit->text();
    _virtualApplianceLocation = ui->_locationLineEdit->text();
    accept();
}

void NewVmDialog::_reject()
{
    reject();
}

//  End of emuone/NewVmDialog.cpp
