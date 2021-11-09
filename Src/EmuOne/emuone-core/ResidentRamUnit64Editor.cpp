//
//  emuone-core/ResidentRamUnit64Editor.cpp
//
//  "Resident RAM unit editor" control
//
//////////
#include "emuone-core/API.hpp"
#include "ui_ResidentRamUnit64Editor.h"
using namespace core;
#include "emuone-core/ResidentRamUnit64Editor.hpp"

//////////
//  Construction/destruction
ResidentRamUnit64Editor::ResidentRamUnit64Editor(ResidentRamUnit64 * ramUnit, QWidget *parent)
    :   ComponentEditor(ramUnit, parent),
        _ui(new Ui::ResidentRamUnit64Editor),
        _ramUnit(ramUnit)
{
    Q_ASSERT(_ramUnit != nullptr);

    _ui->setupUi(this);
    _ui->_sizeUnitComboBox->addItem("B");
    _ui->_sizeUnitComboBox->addItem("KB");
    _ui->_sizeUnitComboBox->addItem("MB");
    _ui->_sizeUnitComboBox->addItem("GB");
}

ResidentRamUnit64Editor::~ResidentRamUnit64Editor()
{
    delete _ui;
}

//////////
//  ComponentEditor
void ResidentRamUnit64Editor::refresh()
{
    _refreshUnderway = true;

    _ui->_addressLineEdit->setText(("0000000000000000" + QString::number(_ramUnit->startAddress(), 16)).right(16).toUpper());
    _ui->_sizeLineEdit->setText(QString::number(_ramUnit->size().numberOfUnits()));
    _ui->_sizeUnitComboBox->setCurrentIndex((int)_ramUnit->size().unit());

    _refreshUnderway = false;
}

//////////
//  Event handlers
void ResidentRamUnit64Editor::_addressLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        bool parsedOk = false;
        uint64_t address = (uint64_t)_ui->_addressLineEdit->text().toULongLong(&parsedOk, 16);
        _ramUnit->setStartAddress(address);
        emit componentConfigurationChanged(component());
    }
}

void ResidentRamUnit64Editor::_sizeLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        core::MemorySize::Unit unit = (core::MemorySize::Unit)_ui->_sizeUnitComboBox->currentIndex();
        uint64_t numberOfUnits = _ui->_sizeLineEdit->text().toUInt();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::MemorySize newSize(unit, numberOfUnits);
            _ramUnit->setSize(newSize);
            emit componentConfigurationChanged(component());
        }
    }
}

void ResidentRamUnit64Editor::_sizeUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        core::MemorySize::Unit unit = (core::MemorySize::Unit)_ui->_sizeUnitComboBox->currentIndex();
        uint64_t numberOfUnits = _ui->_sizeLineEdit->text().toUInt();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::MemorySize newSize(unit, numberOfUnits);
            _ramUnit->setSize(newSize);
            emit componentConfigurationChanged(component());
        }
    }
}

//  End of emuone-core/ResidentRamUnit64Editor.cpp
