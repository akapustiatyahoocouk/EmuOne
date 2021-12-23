//
//  emuone-cereon/RomUnitEditor.cpp
//
//  cereon::RomUnitEditor class implementation
//
//////////
#include "emuone-cereon/API.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-promo"
#include "ui_RomUnitEditor.h"
#pragma GCC diagnostic pop
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

    _refreshUnderway = true;
    _ui->_sizeUnitComboBox->addItem("B");
    _ui->_sizeUnitComboBox->addItem("KB");
    _ui->_sizeUnitComboBox->addItem("MB");
    _ui->_sizeUnitComboBox->addItem("GB");
    _refreshUnderway = false;
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

    _ui->_addressLineEdit->setText(util::toString(_romUnit->startAddress(), "%016X"));
    _ui->_sizeLineEdit->setText(util::toString(_romUnit->size().numberOfUnits()));
    _ui->_sizeUnitComboBox->setCurrentIndex(static_cast<int>(_romUnit->size().unit()));
    _ui->_contentLineEdit->setText(util::toString(_romUnit->contentFileName()));

    _refreshUnderway = false;
}

//////////
//  Implementation helpers
void RomUnitEditor::_applyStartAddressShanges()
{
    uint64_t address = 0;
    if (util::fromString(_ui->_addressLineEdit->text(), "%X", address))
    {   //  Use it
        _romUnit->setStartAddress(address);
        emit componentConfigurationChanged(component());
    }
}

void RomUnitEditor::_applySizeChanges()
{
    core::MemorySize::Unit unit = static_cast<core::MemorySize::Unit>(_ui->_sizeUnitComboBox->currentIndex());
    uint64_t numberOfUnits = _ui->_sizeLineEdit->text().toULongLong();
    if (numberOfUnits > 0 &&
        numberOfUnits * core::getUnitValue(unit) / core::getUnitValue(unit) == numberOfUnits)
    {   //  Parsed successfully AND no overflow
        core::MemorySize newSize(unit, numberOfUnits);
        _romUnit->setSize(newSize);
        emit componentConfigurationChanged(component());
    }
}

void RomUnitEditor::_applyContentFileNameChanges()
{
    _romUnit->setContentFileName(_romUnit->virtualAppliance()->toRelativePath(_ui->_contentLineEdit->text()));
    _ui->_contentLineEdit->setText(_romUnit->contentFileName());
    emit componentConfigurationChanged(component());
}

//////////
//  Event handlers
void RomUnitEditor::_addressLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applyStartAddressShanges();
    }
}

void RomUnitEditor::_sizeLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applySizeChanges();
    }
}

void RomUnitEditor::_sizeUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        _applySizeChanges();
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

void RomUnitEditor::_contentLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applyContentFileNameChanges();
    }
}

//  End of emuone-cereon/RomUnitEditor.cpp
