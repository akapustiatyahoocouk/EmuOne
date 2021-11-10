//
//  emuone-cereon/RomUnitEditor.cpp
//
//  cereon::RomUnitEditor class implementation
//
//////////
#include "emuone-cereon/API.hpp"
#include "ui_RomUnitEditor.h"
using namespace cereon;

//////////
//  Construction/destruction
RomUnitEditor::RomUnitEditor(RomUnit * romUnit, QWidget * parent)
    :   ComponentEditor(romUnit, parent),
        _ui(new Ui::RomUnitEditor),
        _romUnit(romUnit)
{
    Q_ASSERT(_romUnit != nullptr);

    _ui->setupUi(this);
    _ui->_sizeUnitComboBox->addItem("B");
    _ui->_sizeUnitComboBox->addItem("KB");
    _ui->_sizeUnitComboBox->addItem("MB");
    _ui->_sizeUnitComboBox->addItem("GB");
}

RomUnitEditor::~RomUnitEditor()
{
    _romUnit->_editors.removeOne(this);
    delete _ui;
}

//////////
//  core::ComponentEditor
void RomUnitEditor::refresh()
{
    _refreshUnderway = true;

    _ui->_addressLineEdit->setText(_romUnit->startAddressString());
    _ui->_sizeLineEdit->setText(QString::number(_romUnit->size().numberOfUnits()));
    _ui->_sizeUnitComboBox->setCurrentIndex((int)_romUnit->size().unit());
    _ui->_contentLineEdit->setText(_romUnit->contentFileName());

    _refreshUnderway = false;
}

//////////
//  Event handlers
void RomUnitEditor::_addressLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        bool parsedOk = false;
        uint64_t address = (uint64_t)_ui->_addressLineEdit->text().toULongLong(&parsedOk, 16);
        _romUnit->setStartAddress(address);
        emit componentConfigurationChanged(component());
    }
}

void RomUnitEditor::_sizeLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        core::MemorySize::Unit unit = (core::MemorySize::Unit)_ui->_sizeUnitComboBox->currentIndex();
        uint64_t numberOfUnits = _ui->_sizeLineEdit->text().toULongLong();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::MemorySize newSize(unit, numberOfUnits);
            _romUnit->setSize(newSize);
            emit componentConfigurationChanged(component());
        }
    }
}

void RomUnitEditor::_sizeUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        core::MemorySize::Unit unit = (core::MemorySize::Unit)_ui->_sizeUnitComboBox->currentIndex();
        uint64_t numberOfUnits = _ui->_sizeLineEdit->text().toULongLong();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::MemorySize newSize(unit, numberOfUnits);
            _romUnit->setSize(newSize);
            emit componentConfigurationChanged(component());
        }
    }
}

void RomUnitEditor::_browsePushButtonClicked()
{
    QString contentFileName = QFileDialog::getOpenFileName(this, "ROM content", "", "ROM content file (*.bin)");
    if (contentFileName.isEmpty())
    {   //  User has cancelled the dialog
        return;
    }
    _romUnit->setContentFileName(_romUnit->virtualAppliance()->toRelativePath(contentFileName));
    _ui->_contentLineEdit->setText(_romUnit->contentFileName());
}

//  End of emuone-cereon/RomUnitEditor.cpp
