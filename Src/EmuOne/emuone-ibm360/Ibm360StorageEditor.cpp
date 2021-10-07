//
//  emuone-360/Ibm360StorageEditor.cpp
//
//  Ibm360StorageEditor class implementation
//
//////////
#include "emuone-ibm360/API.hpp"
#include "Ibm360StorageEditor.hpp"
#include "ui_Ibm360StorageEditor.h"

using namespace ibm360;

//////////
//  Construction/destruction
Ibm360StorageEditor::Ibm360StorageEditor(Storage * storage, QWidget * parent)
    :   ComponentEditor(storage, parent),
        ui(new Ui::Ibm360StorageEditor),
        _storage(storage)
{
    Q_ASSERT(_storage != nullptr);

    ui->setupUi(this);
    ui->_sizeUnitComboBox->addItem("KB");
    ui->_sizeUnitComboBox->addItem("MB");
}

Ibm360StorageEditor::~Ibm360StorageEditor()
{
    delete ui;
}

//////////
//  ComponentEditor
void Ibm360StorageEditor::refresh()
{
    _refreshUnderway = true;

    switch (_storage->_size.getUnit())
    {
        case core::MemorySize::Unit::KB:
            ui->_sizeUnitComboBox->setCurrentIndex(0);
            ui->_sizeLineEdit->setText(QString::number(_storage->_size.getNumberOfUnits()));
            break;
        case core::MemorySize::Unit::MB:
            ui->_sizeUnitComboBox->setCurrentIndex(1);
            ui->_sizeLineEdit->setText(QString::number(_storage->_size.getNumberOfUnits()));
            break;
        default:
            ui->_sizeUnitComboBox->setCurrentIndex(0);  //  KB
            ui->_sizeLineEdit->setText(QString::number(_storage->_size.toUnits(core::MemorySize::Unit::KB)));
            break;
    }

    _refreshUnderway = false;
}

//////////
//  Event handlers
void Ibm360StorageEditor::_sizeLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        core::MemorySize::Unit unit = (ui->_sizeUnitComboBox->currentIndex() == 0) ? core::MemorySize::Unit::KB : core::MemorySize::Unit::MB;
        uint64_t numberOfUnits = ui->_sizeLineEdit->text().toUInt();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::MemorySize newSize(unit, numberOfUnits);
            if (Storage::isValidSize(newSize))
            {   //  Use it
                _storage->_size = newSize;
                emit componentConfigurationChanged(getComponent());
            }
        }
    }
}

void Ibm360StorageEditor::_sizeUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        core::MemorySize::Unit unit = (ui->_sizeUnitComboBox->currentIndex() == 0) ? core::MemorySize::Unit::KB : core::MemorySize::Unit::MB;
        uint64_t numberOfUnits = ui->_sizeLineEdit->text().toUInt();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::MemorySize newSize(unit, numberOfUnits);
            if (Storage::isValidSize(newSize))
            {   //  Use it
                _storage->_size = newSize;
                emit componentConfigurationChanged(getComponent());
            }
        }
    }
}

//  End of emuone-360/Ibm360StorageEditor.cpp
