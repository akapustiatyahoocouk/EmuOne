//
//  emuone-cereon/Cmos1Editor.cpp
//
//  cereon::Cmos1Editor class implementation
//
//////////
#include "emuone-cereon/API.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-promo"
#include "ui_Cmos1Editor.h"
#pragma GCC diagnostic pop
using namespace cereon;

//////////
//  Construction/destruction
Cmos1Editor::Cmos1Editor(Cmos1 * cmos1, QWidget * parent)
    :   core::ComponentEditor(cmos1, parent),
        _ui(new Ui::Cmos1Editor),
        _cmos1(cmos1)
{
    _ui->setupUi(this);
    Q_ASSERT(cmos1 != nullptr);

    //  Populate the "clock frequency unit" drop-down list
    _ui->_clockFrequencyUnitComboBox->addItem("Hz");
    _ui->_clockFrequencyUnitComboBox->addItem("KHz");
    _ui->_clockFrequencyUnitComboBox->addItem("MHz");
    _ui->_clockFrequencyUnitComboBox->addItem("GHz");

    //  Populate the "read delay unit" drop-down list
    _ui->_readDelayUnitComboBox->addItem("s");
    _ui->_readDelayUnitComboBox->addItem("ms");
    _ui->_readDelayUnitComboBox->addItem("us");
    _ui->_readDelayUnitComboBox->addItem("nx");

    //  Populate the "write delay unit" drop-down list
    _ui->_writeDelayUnitComboBox->addItem("s");
    _ui->_writeDelayUnitComboBox->addItem("ms");
    _ui->_writeDelayUnitComboBox->addItem("us");
    _ui->_writeDelayUnitComboBox->addItem("nx");
}

Cmos1Editor::~Cmos1Editor()
{
    delete _ui;
}

//////////
//  core::ComponentEditor
void Cmos1Editor::refresh()
{
    _refreshUnderway = true;

    _ui->_statePortLineEdit->setText(("0000" + QString::number(static_cast<unsigned>(_cmos1->statePortAddress()), 16)).right(4).toUpper());
    _ui->_addressPortLineEdit->setText(("0000" + QString::number(static_cast<unsigned>(_cmos1->addressPortAddress()), 16)).right(4).toUpper());
    _ui->_dataPortLineEdit->setText(("0000" + QString::number(static_cast<unsigned>(_cmos1->dataPortAddress()), 16)).right(4).toUpper());
    _ui->_interruptMaskPortLineEdit->setText(("0000" + QString::number(static_cast<unsigned>(_cmos1->interruptMaskPortAddress()), 16)).right(4).toUpper());
    _ui->_clockFrequencyValueLineEdit->setText(QString::number(_cmos1->clockFrequency().numberOfUnits()));
    _ui->_clockFrequencyUnitComboBox->setCurrentIndex(static_cast<int>(_cmos1->clockFrequency().unit()));
    _ui->_readDelayValueLineEdit->setText(QString::number(_cmos1->readDelay().numberOfUnits()));
    _ui->_readDelayUnitComboBox->setCurrentIndex(static_cast<int>(_cmos1->readDelay().unit()));
    _ui->_writeDelayValueLineEdit->setText(QString::number(_cmos1->writeDelay().numberOfUnits()));
    _ui->_writeDelayUnitComboBox->setCurrentIndex(static_cast<int>(_cmos1->writeDelay().unit()));
    _ui->_contentLineEdit->setText(_cmos1->contentFilePath());

    _refreshUnderway = false;
}

//////////
//  Event listeners
void Cmos1Editor::_statePortLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        bool parsedOk = false;
        uint16_t address = static_cast<uint16_t>(_ui->_statePortLineEdit->text().toUInt(&parsedOk, 16));
        _cmos1->setStatePortAddress(address);
        emit componentConfigurationChanged(component());
    }
}

void Cmos1Editor::_addressPortLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        bool parsedOk = false;
        uint16_t address = static_cast<uint16_t>(_ui->_addressPortLineEdit->text().toUInt(&parsedOk, 16));
        _cmos1->setAddressPortAddress(address);
        emit componentConfigurationChanged(component());
    }
}

void Cmos1Editor::_dataPortLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        bool parsedOk = false;
        uint16_t address = static_cast<uint16_t>(_ui->_dataPortLineEdit->text().toUInt(&parsedOk, 16));
        _cmos1->setDataPortAddress(address);
        emit componentConfigurationChanged(component());
    }
}

void Cmos1Editor::_interruptMaskPortLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        bool parsedOk = false;
        uint16_t address = static_cast<uint16_t>(_ui->_interruptMaskPortLineEdit->text().toUInt(&parsedOk, 16));
        _cmos1->setInterruptMaskPortAddress(address);
        emit componentConfigurationChanged(component());
    }
}

//  End of emuone-cereon/Cmos1Editor.cpp
