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
    _ui->_readDelayUnitComboBox->addItem("ns");

    //  Populate the "write delay unit" drop-down list
    _ui->_writeDelayUnitComboBox->addItem("s");
    _ui->_writeDelayUnitComboBox->addItem("ms");
    _ui->_writeDelayUnitComboBox->addItem("us");
    _ui->_writeDelayUnitComboBox->addItem("ns");
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
//  Implementation helpers
void Cmos1Editor::_applyStatePortAddressChanges()
{
    uint16_t address;
    if (util::fromString(_ui->_statePortLineEdit->text(), "%x", address))
    {
        _cmos1->setStatePortAddress(address);
    }
    emit componentConfigurationChanged(component());
}

void Cmos1Editor::_applyAddressPortAddressChanges()
{
    uint16_t address;
    if (util::fromString(_ui->_addressPortLineEdit->text(), "%x", address))
    {
        _cmos1->setAddressPortAddress(address);
    }
    emit componentConfigurationChanged(component());
}

void Cmos1Editor::_applyDataPortAddressChanges()
{
    uint16_t address;
    if (util::fromString(_ui->_dataPortLineEdit->text(), "%x", address))
    {
        _cmos1->setDataPortAddress(address);
    }
    emit componentConfigurationChanged(component());
}

void Cmos1Editor::_applyInterruptMaskPortAddressChanges()
{
    uint16_t address;
    if (util::fromString(_ui->_interruptMaskPortLineEdit->text(), "%x", address))
    {
        _cmos1->setInterruptMaskPortAddress(address);
    }
    emit componentConfigurationChanged(component());
}

void Cmos1Editor::_applyReadDelayChanges()
{
    uint64_t numberOfUnits = _ui->_writeDelayValueLineEdit->text().toULongLong();
    core::Duration::Unit unit = static_cast<core::Duration::Unit>(_ui->_writeDelayUnitComboBox->currentIndex());
    if (numberOfUnits > 0 &&
        numberOfUnits * core::getUnitValue(unit) / core::getUnitValue(unit) == numberOfUnits)
    {   //  Parsing successfuol AND no overflow
        _cmos1->setWriteDelay(core::Duration(unit, numberOfUnits));
    }
    emit componentConfigurationChanged(component());
}

void Cmos1Editor::_applyWriteDelayChanges()
{
    uint64_t numberOfUnits = _ui->_readDelayValueLineEdit->text().toULongLong();
    core::Duration::Unit unit = static_cast<core::Duration::Unit>(_ui->_readDelayUnitComboBox->currentIndex());
    if (numberOfUnits > 0 &&
        numberOfUnits * core::getUnitValue(unit) / core::getUnitValue(unit) == numberOfUnits)
    {   //  Parsing successfuol AND no overflow
        _cmos1->setReadDelay(core::Duration(unit, numberOfUnits));
    }
    emit componentConfigurationChanged(component());
}

void Cmos1Editor::_applyClockFrequencyChanges()
{
    uint64_t numberOfUnits = _ui->_clockFrequencyValueLineEdit->text().toULongLong();
    core::ClockFrequency::Unit unit = static_cast<core::ClockFrequency::Unit>(_ui->_clockFrequencyUnitComboBox->currentIndex());
    if (numberOfUnits > 0 &&
        numberOfUnits * core::getUnitValue(unit) / core::getUnitValue(unit) == numberOfUnits)
    {   //  Parsing successful AND no overflow
        _cmos1->setClockFrequency(core::ClockFrequency(unit, numberOfUnits));
    }
    emit componentConfigurationChanged(component());
}

void Cmos1Editor::_browseForContentFileName()
{
    QString contentFileName = QFileDialog::getOpenFileName(this, "CMOS content", "", "CMOS content file (*.bin)");
    if (contentFileName.isEmpty())
    {   //  User has cancelled the dialog
        return;
    }
    _cmos1->setContentFilePath(_cmos1->virtualAppliance()->toRelativePath(contentFileName));
    _ui->_contentLineEdit->setText(_cmos1->contentFilePath());
}

void Cmos1Editor::_applyContentFilePathChanges()
{
    _cmos1->setContentFilePath(_ui->_contentLineEdit->text().trimmed());
}

//////////
//  Event listeners
void Cmos1Editor::_statePortLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applyStatePortAddressChanges();
    }
}

void Cmos1Editor::_addressPortLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applyAddressPortAddressChanges();
    }
}

void Cmos1Editor::_dataPortLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applyDataPortAddressChanges();
    }
}

void Cmos1Editor::_interruptMaskPortLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applyInterruptMaskPortAddressChanges();
    }
}

void Cmos1Editor::_readDelayValueLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applyReadDelayChanges();
    }
}

void Cmos1Editor::_writeDelayValueLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applyWriteDelayChanges();
    }
}

void Cmos1Editor::_readDelayUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        _applyReadDelayChanges();
    }
}

void Cmos1Editor::_writeDelayUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        _applyWriteDelayChanges();
    }
}

void Cmos1Editor::_clockFrequencyValueLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applyClockFrequencyChanges();
    }
}

void Cmos1Editor::_clockFrequencyUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        _applyClockFrequencyChanges();
    }
}

void Cmos1Editor::_browsePushButtonClicked()
{
    _browseForContentFileName();
}

void Cmos1Editor::_contentLineEditLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applyContentFilePathChanges();
    }
}

//  End of emuone-cereon/Cmos1Editor.cpp
