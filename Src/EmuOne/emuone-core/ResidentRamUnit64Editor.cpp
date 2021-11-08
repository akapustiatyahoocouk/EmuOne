//
//  emuone-core/ResidentRamUnit64Editor.cpp
//
//  The editor of the ResidentRamUnit64 components
//
//////////
#include "emuone-core/API.hpp"
#include "ui_ResidentRamUnit64Editor.h"
#include "emuone-core/ResidentRamUnit64Editor.hpp"

using namespace core;

//////////
//  Construction/destruction
ResidentRamUnit64Editor::ResidentRamUnit64Editor(ResidentRamUnit64 * ramInit, QWidget * parent)
    :   ComponentEditor(ramInit, parent),
        _ui(new Ui::ResidentRamUnit64Editor),
        _ramInit(ramInit)
{
    Q_ASSERT(_ramInit != nullptr);

    _ui->setupUi(this);
    _ui->_sizeUnitComboBox->addItem("B");
    _ui->_sizeUnitComboBox->addItem("KB");
    _ui->_sizeUnitComboBox->addItem("MB");
    _ui->_sizeUnitComboBox->addItem("GB");
}

ResidentRamUnit64Editor::~ResidentRamUnit64Editor()
{
    _ramInit->_editors.removeOne(this);
    delete _ui;
}

//////////
//  ComponentEditor
void ResidentRamUnit64Editor::refresh()
{
    _refreshUnderway = true;

    _ui->_addressLineEdit->setText(("0000000000000000" + QString::number(_ramInit->startAddress(), 16)).toUpper().right(16));

    switch (_ramInit->size().unit())
    {
        case core::MemorySize::Unit::B:
            _ui->_sizeUnitComboBox->setCurrentIndex(0);
            _ui->_sizeLineEdit->setText(QString::number(_ramInit->size().numberOfUnits()));
            break;
        case core::MemorySize::Unit::KB:
            _ui->_sizeUnitComboBox->setCurrentIndex(1);
            _ui->_sizeLineEdit->setText(QString::number(_ramInit->size().numberOfUnits()));
            break;
        case core::MemorySize::Unit::MB:
            _ui->_sizeUnitComboBox->setCurrentIndex(2);
            _ui->_sizeLineEdit->setText(QString::number(_ramInit->size().numberOfUnits()));
            break;
        case core::MemorySize::Unit::GB:
            _ui->_sizeUnitComboBox->setCurrentIndex(3);
            _ui->_sizeLineEdit->setText(QString::number(_ramInit->size().numberOfUnits()));
            break;
        default:
            _ui->_sizeUnitComboBox->setCurrentIndex(0);  //  MB
            _ui->_sizeLineEdit->setText(QString::number(_ramInit->size().toUnits(core::MemorySize::Unit::MB)));
            break;
    }

    _refreshUnderway = false;
}

//  End of emuone-core/ResidentRamUnit64Editor.cpp
