//
//  emuone-360/Ibm360StorageEditor.cpp
//
//  Ibm360StorageEditor class implementation
//
//////////
#include "emuone-ibm360/API.hpp"
#include "Ibm360StorageEditor.hpp"
#include "ui_Ibm360StorageEditor.h"

//////////
//  Construction/destruction
Ibm360StorageEditor::Ibm360StorageEditor(Ibm360Storage * ibm360Storage, QWidget * parent)
    :   ComponentEditor(ibm360Storage, parent),
        ui(new Ui::Ibm360StorageEditor),
        _ibm360Storage(ibm360Storage)
{
    Q_ASSERT(_ibm360Storage != nullptr);

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

    switch (_ibm360Storage->_size.getUnit())
    {
        case MemorySize::Unit::KB:
            ui->_sizeUnitComboBox->setCurrentIndex(0);
            ui->_sizeLineEdit->setText(QString::number(_ibm360Storage->_size.getNumberOfUnits()));
            break;
        case MemorySize::Unit::MB:
            ui->_sizeUnitComboBox->setCurrentIndex(1);
            ui->_sizeLineEdit->setText(QString::number(_ibm360Storage->_size.getNumberOfUnits()));
            break;
        default:
            ui->_sizeUnitComboBox->setCurrentIndex(0);  //  KB
            ui->_sizeLineEdit->setText(QString::number(_ibm360Storage->_size.toUnits(MemorySize::Unit::KB)));
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
        MemorySize::Unit unit = (ui->_sizeUnitComboBox->currentIndex() == 0) ? MemorySize::Unit::KB : MemorySize::Unit::MB;
        uint64_t numberOfUnits = ui->_sizeLineEdit->text().toUInt();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            MemorySize newSize(unit, numberOfUnits);
            if (Ibm360Storage::isValidSize(newSize))
            {   //  Use it
                _ibm360Storage->_size = newSize;
                emit componentConfigurationChanged(getComponent());
            }
        }
    }
}

void Ibm360StorageEditor::_sizeUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        MemorySize::Unit unit = (ui->_sizeUnitComboBox->currentIndex() == 0) ? MemorySize::Unit::KB : MemorySize::Unit::MB;
        uint64_t numberOfUnits = ui->_sizeLineEdit->text().toUInt();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            MemorySize newSize(unit, numberOfUnits);
            if (Ibm360Storage::isValidSize(newSize))
            {   //  Use it
                _ibm360Storage->_size = newSize;
                emit componentConfigurationChanged(getComponent());
            }
        }
    }
}

//  End of emuone-360/Ibm360StorageEditor.cpp
