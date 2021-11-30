//
//  emuone-cereon/RamUnitEditor.cpp
//
//  cereon::RamUnitEditor class implementation
//
//////////
#include "emuone-cereon/API.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-promo"
#include "ui_RamUnitEditor.h"
#pragma GCC diagnostic pop
using namespace cereon;

//////////
//  Construction/destruction
RamUnitEditor::RamUnitEditor(RamUnit * ramUnit, QWidget * parent)
    :   ComponentEditor(ramUnit, parent),
        _ui(new Ui::RamUnitEditor),
        _ramUnit(ramUnit)
{
    Q_ASSERT(_ramUnit != nullptr);

    _ui->setupUi(this);

    _refreshUnderway = true;
    _ui->_sizeUnitComboBox->addItem("B");
    _ui->_sizeUnitComboBox->addItem("KB");
    _ui->_sizeUnitComboBox->addItem("MB");
    _ui->_sizeUnitComboBox->addItem("GB");
    _refreshUnderway = false;
}

RamUnitEditor::~RamUnitEditor()
{
    _ramUnit->_editors.removeOne(this);
    delete _ui;
}

//////////
//  core::ComponentEditor
void RamUnitEditor::refresh()
{
    _refreshUnderway = true;

    _ui->_addressLineEdit->setText(_ramUnit->startAddressString());
    _ui->_sizeLineEdit->setText(QString::number(_ramUnit->size().numberOfUnits()));
    _ui->_sizeUnitComboBox->setCurrentIndex(static_cast<int>(_ramUnit->size().unit()));

    _refreshUnderway = false;
}

//////////
//  Event handlers
void RamUnitEditor::_addressLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        bool parsedOk = false;
        uint64_t address = _ui->_addressLineEdit->text().toULongLong(&parsedOk, 16);
        _ramUnit->setStartAddress(address);
        emit componentConfigurationChanged(component());
    }
}

void RamUnitEditor::_sizeLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        core::MemorySize::Unit unit = static_cast<core::MemorySize::Unit>(_ui->_sizeUnitComboBox->currentIndex());
        uint64_t numberOfUnits = _ui->_sizeLineEdit->text().toULongLong();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::MemorySize newSize(unit, numberOfUnits);
            _ramUnit->setSize(newSize);
            emit componentConfigurationChanged(component());
        }
    }
}

void RamUnitEditor::_sizeUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        core::MemorySize::Unit unit = static_cast<core::MemorySize::Unit>(_ui->_sizeUnitComboBox->currentIndex());
        uint64_t numberOfUnits = _ui->_sizeLineEdit->text().toULongLong();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::MemorySize newSize(unit, numberOfUnits);
            _ramUnit->setSize(newSize);
            emit componentConfigurationChanged(component());
        }
    }
}

//  End of emuone-cereon/RamUnitEditor.cpp
