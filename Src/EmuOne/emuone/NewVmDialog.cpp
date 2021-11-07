//
//  emuone/NewVmDialog.cpp
//
//  "New VM" dialog
//
//////////
#include "emuone/API.hpp"
#include "ui_NewVmDialog.h"

//////////
//  Construction/destruction
NewVmDialog::NewVmDialog(QWidget * parent)
    :   QDialog(parent),
        _ui(new Ui::NewVmDialog)
{
    _ui->setupUi(this);

    //  Populate "architectures" combo box
    core::ArchitectureList architectures = core::Architecture::all();
    if (architectures.isEmpty())
    {   //  No architectures - can't do anything really
        _ui->_templateLabel->setEnabled(false);
        _ui->_templateComboBox->setEnabled(false);
        _ui->_nameLabel->setEnabled(false);
        _ui->_nameLineEdit->setEnabled(false);
        _ui->_locationLabel->setEnabled(false);
        _ui->_locationLineEdit->setEnabled(false);
        _ui->_browseButton->setEnabled(false);
    }
    else
    {   //  SOME architectures are defined
        for (core::Architecture * architecture : architectures)
        {
            _ui->_architectureComboBox->addItem(architecture->smallIcon(), architecture->displayName(), QVariant::fromValue((void*)architecture));
        }
        _ui->_architectureComboBox->setCurrentIndex(0);
    }

    //  Done
    _refreshTemplatesList();
    _refresh();
}

NewVmDialog::~NewVmDialog()
{
    delete _ui;
}

//////////
//  Implementation helpers
void NewVmDialog::_refresh()
{
    _ui->_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(_getSelectedArchitecture() != nullptr &&
                                                             _getSelectedTemplate() != nullptr &&
                                                             core::VirtualAppliance::isValidName(_ui->_nameLineEdit->text()) &&
                                                             !_ui->_locationLineEdit->text().isEmpty());
}

void NewVmDialog::_refreshTemplatesList()
{
    core::VirtualApplianceTemplateList templateList;
    core::Architecture * architecture = _getSelectedArchitecture();
    if (architecture != nullptr)
    {
        if (_ui->_virtualMachineRadioButton->isChecked())
        {
            for (core::VirtualMachineTemplate * virtualMachineTemplate : architecture->virtualMachineTemplates())
            {
                templateList.append(virtualMachineTemplate);
            }
        }
        else
        {
            for (core::RemoteTerminalTemplate * remoteTerminalTemplate : architecture->remoteTerminalTemplates())
            {
                templateList.append(remoteTerminalTemplate);
            }
        }
    }
    _ui->_templateComboBox->clear();
    if (templateList.isEmpty())
    {   //  OOPS! No templates available!
        _ui->_templateLabel->setEnabled(false);
        _ui->_templateComboBox->setEnabled(false);
        _ui->_nameLabel->setEnabled(false);
        _ui->_nameLineEdit->setEnabled(false);
        _ui->_locationLabel->setEnabled(false);
        _ui->_locationLineEdit->setEnabled(false);
        _ui->_browseButton->setEnabled(false);
    }
    else
    {   //  SOME templates available
        _ui->_templateLabel->setEnabled(true);
        _ui->_templateComboBox->setEnabled(true);
        _ui->_nameLabel->setEnabled(true);
        _ui->_nameLineEdit->setEnabled(true);
        _ui->_locationLabel->setEnabled(true);
        _ui->_locationLineEdit->setEnabled(true);
        _ui->_browseButton->setEnabled(true);
        for (core::VirtualApplianceTemplate * virtualApplianceTemplate : templateList)
        {
            _ui->_templateComboBox->addItem(virtualApplianceTemplate->smallIcon(),
                                            virtualApplianceTemplate->displayName(),
                                            QVariant::fromValue((void*)virtualApplianceTemplate));
        }
        _ui->_templateComboBox->setCurrentIndex(0);
    }
}

core::Architecture * NewVmDialog::_getSelectedArchitecture()
{
    int n = _ui->_architectureComboBox->currentIndex();
    if (n >= 0)
    {
        return (core::Architecture*)(_ui->_architectureComboBox->itemData(n).value<void*>());
    }
    return nullptr;
}

core::VirtualApplianceTemplate * NewVmDialog::_getSelectedTemplate()
{
    int n = _ui->_templateComboBox->currentIndex();
    if (n >= 0)
    {
        return (core::VirtualApplianceTemplate*)(_ui->_templateComboBox->itemData(n).value<void*>());
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
    _ui->_locationLineEdit->setText(location);
}

void NewVmDialog::_accept()
{
    _virtualApplianceType =
            _ui->_virtualMachineRadioButton->isChecked() ?
                (core::VirtualApplianceType*)core::VirtualMachine::Type::instance() :
                (core::VirtualApplianceType*)core::RemoteTerminal::Type::instance();
    _virtualApplianceArchitecture = _getSelectedArchitecture();
    _virtualApplianceTemplate = _getSelectedTemplate();
    _virtualApplianceName = _ui->_nameLineEdit->text();
    _virtualApplianceLocation = _ui->_locationLineEdit->text();
    accept();
}

void NewVmDialog::_reject()
{
    reject();
}

//  End of emuone/NewVmDialog.cpp
