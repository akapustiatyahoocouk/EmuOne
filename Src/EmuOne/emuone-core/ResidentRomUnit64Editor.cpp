//
//  emuone-core/ResidentRomUnit64Editor.cpp
//
//  "Resident ROM unit editor" control
//
//////////
#include "emuone-core/API.hpp"
#include "ui_ResidentRomUnit64Editor.h"
using namespace core;
#include "emuone-core/ResidentRomUnit64Editor.hpp"

//////////
//  Construction/destruction
ResidentRomUnit64Editor::ResidentRomUnit64Editor(ResidentRomUnit64 * romUnit, QWidget * parent)
    :   ComponentEditor(romUnit, parent),
        _ui(new Ui::ResidentRomUnit64Editor),
        _romUnit(romUnit)
{
    Q_ASSERT(_romUnit != nullptr);

    _ui->setupUi(this);
    _ui->_sizeUnitComboBox->addItem("B");
    _ui->_sizeUnitComboBox->addItem("KB");
    _ui->_sizeUnitComboBox->addItem("MB");
    _ui->_sizeUnitComboBox->addItem("GB");
}

ResidentRomUnit64Editor::~ResidentRomUnit64Editor()
{
    delete _ui;
}

//////////
//  ComponentEditor
void ResidentRomUnit64Editor::refresh()
{
    _refreshUnderway = true;

    _ui->_addressLineEdit->setText(("0000000000000000" + QString::number(_romUnit->startAddress(), 16)).right(16).toUpper());
    _ui->_sizeLineEdit->setText(QString::number(_romUnit->size().numberOfUnits()));
    _ui->_sizeUnitComboBox->setCurrentIndex((int)_romUnit->size().unit());
    _ui->_contentLineEdit->setText(_romUnit->contentFileName());

    _refreshUnderway = false;
}

//////////
//  Event handlers
void ResidentRomUnit64Editor::_addressLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        bool parsedOk = false;
        uint64_t address = (uint64_t)_ui->_addressLineEdit->text().toULongLong(&parsedOk, 16);
        _romUnit->setStartAddress(address);
        emit componentConfigurationChanged(component());
    }
}

void ResidentRomUnit64Editor::_sizeLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        core::MemorySize::Unit unit = (core::MemorySize::Unit)_ui->_sizeUnitComboBox->currentIndex();
        uint64_t numberOfUnits = _ui->_sizeLineEdit->text().toUInt();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::MemorySize newSize(unit, numberOfUnits);
            _romUnit->setSize(newSize);
            emit componentConfigurationChanged(component());
        }
    }
}

void ResidentRomUnit64Editor::_sizeUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        core::MemorySize::Unit unit = (core::MemorySize::Unit)_ui->_sizeUnitComboBox->currentIndex();
        uint64_t numberOfUnits = _ui->_sizeLineEdit->text().toUInt();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::MemorySize newSize(unit, numberOfUnits);
            _romUnit->setSize(newSize);
            emit componentConfigurationChanged(component());
        }
    }
}

void ResidentRomUnit64Editor::_browsePushButtonClicked()
{
    QString location = QFileDialog::getOpenFileName(this->topLevelWidget(), "ROM content", "", "ROM content file (*.bin)");
    if (location.isEmpty())
    {   //  User has cancelled the dialog
        return;
    }
    _romUnit->setContentFileName(_romUnit->virtualAppliance()->toRelativePath(location));
    _ui->_contentLineEdit->setText(_romUnit->contentFileName());
    emit componentConfigurationChanged(component());
}

//  End of emuone-core/ResidentRomUnit64Editor.cpp

