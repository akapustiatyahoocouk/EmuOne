//
//  emuone-360/Ibm360StorageEditor.cpp
//
//  Ibm360StorageEditor class implementation
//
//////////
#include "emuone-ibm360/API.hpp"
#include "ui_Ibm360StorageEditor.h"
using namespace ibm360;

//////////
//  Construction/destruction
Ibm360StorageEditor::Ibm360StorageEditor(Storage * storage, QWidget * parent)
    :   ComponentEditor(storage, parent),
        _ui(new Ui::Ibm360StorageEditor),
        _storage(storage)
{
    Q_ASSERT(_storage != nullptr);

    _ui->setupUi(this);
    _ui->_sizeUnitComboBox->addItem("KB");
    _ui->_sizeUnitComboBox->addItem("MB");
}

Ibm360StorageEditor::~Ibm360StorageEditor()
{
    delete _ui;
}

//////////
//  ComponentEditor
void Ibm360StorageEditor::refresh()
{
    _refreshUnderway = true;

    switch (_storage->_size.unit())
    {
        case core::MemorySize::Unit::KB:
            _ui->_sizeUnitComboBox->setCurrentIndex(0);
            _ui->_sizeLineEdit->setText(QString::number(_storage->_size.numberOfUnits()));
            break;
        case core::MemorySize::Unit::MB:
            _ui->_sizeUnitComboBox->setCurrentIndex(1);
            _ui->_sizeLineEdit->setText(QString::number(_storage->_size.numberOfUnits()));
            break;
        default:
            _ui->_sizeUnitComboBox->setCurrentIndex(0);  //  KB
            _ui->_sizeLineEdit->setText(QString::number(_storage->_size.toUnits(core::MemorySize::Unit::KB)));
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
        core::MemorySize::Unit unit = (_ui->_sizeUnitComboBox->currentIndex() == 0) ? core::MemorySize::Unit::KB : core::MemorySize::Unit::MB;
        uint64_t numberOfUnits = _ui->_sizeLineEdit->text().toUInt();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::MemorySize newSize(unit, numberOfUnits);
            if (Storage::isValidSize(newSize))
            {   //  Use it
                _storage->_size = newSize;
                emit componentConfigurationChanged(component());
            }
        }
    }
}

void Ibm360StorageEditor::_sizeUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        core::MemorySize::Unit unit = (_ui->_sizeUnitComboBox->currentIndex() == 0) ? core::MemorySize::Unit::KB : core::MemorySize::Unit::MB;
        uint64_t numberOfUnits = _ui->_sizeLineEdit->text().toUInt();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::MemorySize newSize(unit, numberOfUnits);
            if (Storage::isValidSize(newSize))
            {   //  Use it
                _storage->_size = newSize;
                emit componentConfigurationChanged(component());
            }
        }
    }
}

//  End of emuone-360/Ibm360StorageEditor.cpp
