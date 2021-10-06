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

//  End of emuone-360/Ibm360StorageEditor.cpp
