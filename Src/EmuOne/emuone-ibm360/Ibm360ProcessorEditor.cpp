//
//  emuone-360/Ibm360ProcessorEditor.cpp
//
//  Ibm360ProcessorEditor class implementation
//
//////////
#include "emuone-ibm360/API.hpp"
#include "ui_Ibm360ProcessorEditor.h"
using namespace ibm360;

//////////
//  Construction/destruction
Ibm360ProcessorEditor::Ibm360ProcessorEditor(Processor * processor, QWidget * parent)
    :   ComponentEditor(processor, parent),
        _ui(new Ui::Ibm360ProcessorEditor),
        _processor(processor)
{
    Q_ASSERT(_processor != nullptr);

    _ui->setupUi(this);
    _ui->_clockFrequencyUnitComboBox->addItem("HZ");
    _ui->_clockFrequencyUnitComboBox->addItem("KHZ");
    _ui->_clockFrequencyUnitComboBox->addItem("MHZ");
    _ui->_clockFrequencyUnitComboBox->addItem("GHZ");
}

Ibm360ProcessorEditor::~Ibm360ProcessorEditor()
{
    delete _ui;
}

//////////
//  ComponentEditor
void Ibm360ProcessorEditor::refresh()
{
    _refreshUnderway = true;

    switch (_processor->_clockFrequency.unit())
    {
        case core::ClockFrequency::Unit::HZ:
            _ui->_clockFrequencyUnitComboBox->setCurrentIndex(0);
            _ui->_clockFrequencyLineEdit->setText(QString::number(_processor->_clockFrequency.numberOfUnits()));
            break;
        case core::ClockFrequency::Unit::KHZ:
            _ui->_clockFrequencyUnitComboBox->setCurrentIndex(1);
            _ui->_clockFrequencyLineEdit->setText(QString::number(_processor->_clockFrequency.numberOfUnits()));
            break;
        case core::ClockFrequency::Unit::MHZ:
            _ui->_clockFrequencyUnitComboBox->setCurrentIndex(2);
            _ui->_clockFrequencyLineEdit->setText(QString::number(_processor->_clockFrequency.numberOfUnits()));
            break;
        case core::ClockFrequency::Unit::GHZ:
            _ui->_clockFrequencyUnitComboBox->setCurrentIndex(3);
            _ui->_clockFrequencyLineEdit->setText(QString::number(_processor->_clockFrequency.numberOfUnits()));
            break;
    }

    _ui->_fetchProtectionCheckBox->setChecked(has(_processor->features(), Features::FetchProtection));
    _ui->_storeProtectionCheckBox->setChecked(has(_processor->features(), Features::StoreProtection));
    _ui->_decimalCheckBox->setChecked(has(_processor->features(), Features::Decimal));
    _ui->_floatingPointCheckBox->setChecked(has(_processor->features(), Features::FloatingPoint));
    _ui->_byteOrientedOperandCheckBox->setChecked(has(_processor->features(), Features::ByteOrientedOperand));
    _ui->_timerCheckBox->setChecked(has(_processor->features(), Features::Timer));
    _ui->_directControlCheckBox->setChecked(has(_processor->features(), Features::DirectControl));

    _refreshUnderway = false;
}

//////////
//  Event handlers
void Ibm360ProcessorEditor::_clockFrequencyLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        core::ClockFrequency::Unit unit = (core::ClockFrequency::Unit)_ui->_clockFrequencyUnitComboBox->currentIndex();
        uint64_t numberOfUnits = _ui->_clockFrequencyLineEdit->text().toUInt();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::ClockFrequency newClockFrequency(unit, numberOfUnits);
            if (Processor::isValidClockFrequency(newClockFrequency))
            {   //  Use it
                _processor->_clockFrequency = newClockFrequency;
                emit componentConfigurationChanged(component());
            }
        }
    }
}

void Ibm360ProcessorEditor::_clockFrequencyUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        core::ClockFrequency::Unit unit = (core::ClockFrequency::Unit)_ui->_clockFrequencyUnitComboBox->currentIndex();
        uint64_t numberOfUnits = _ui->_clockFrequencyLineEdit->text().toUInt();
        if (numberOfUnits > 0)
        {   //  Parsed successfully
            core::ClockFrequency newClockFrequency(unit, numberOfUnits);
            if (Processor::isValidClockFrequency(newClockFrequency))
            {   //  Use it
                _processor->_clockFrequency = newClockFrequency;
                emit componentConfigurationChanged(component());
            }
        }
    }
}

void Ibm360ProcessorEditor::_fetchProtectionCheckBoxClicked()
{
    if (_ui->_fetchProtectionCheckBox->isChecked())
    {
        _processor->_features |= Features::FetchProtection | Features::StoreProtection;
        _ui->_storeProtectionCheckBox->setChecked(true);
    }
    else
    {
        _processor->_features &= ~Features::FetchProtection;
    }
    emit componentConfigurationChanged(component());
}

void Ibm360ProcessorEditor::_storeProtectionCheckBoxClicked()
{
    if (_ui->_storeProtectionCheckBox->isChecked())
    {
        _processor->_features |= Features::StoreProtection;
    }
    else
    {
        _processor->_features &= ~(Features::FetchProtection | Features::StoreProtection);
        _ui->_fetchProtectionCheckBox->setChecked(false);
    }
    emit componentConfigurationChanged(component());
}

void Ibm360ProcessorEditor::_decimalCheckBoxClicked()
{
    if (_ui->_decimalCheckBox->isChecked())
    {
        _processor->_features |= Features::Decimal;
    }
    else
    {
        _processor->_features &= ~Features::Decimal;
    }
    emit componentConfigurationChanged(component());
}

void Ibm360ProcessorEditor::_floatingPointCheckBoxClicked()
{
    if (_ui->_floatingPointCheckBox->isChecked())
    {
        _processor->_features |= Features::FloatingPoint;
    }
    else
    {
        _processor->_features &= ~Features::FloatingPoint;
    }
    emit componentConfigurationChanged(component());
}

void Ibm360ProcessorEditor::_byteOrientedOperandCheckBoxClicked()
{
    if (_ui->_byteOrientedOperandCheckBox->isChecked())
    {
        _processor->_features |= Features::ByteOrientedOperand;
    }
    else
    {
        _processor->_features &= ~Features::ByteOrientedOperand;
    }
    emit componentConfigurationChanged(component());
}

void Ibm360ProcessorEditor::_timerCheckBoxClicked()
{
    if (_ui->_timerCheckBox->isChecked())
    {
        _processor->_features |= Features::Timer;
    }
    else
    {
        _processor->_features &= ~Features::Timer;
    }
    emit componentConfigurationChanged(component());
}

void Ibm360ProcessorEditor::_directControlCheckBoxClicked()
{
    if (_ui->_directControlCheckBox->isChecked())
    {
        _processor->_features |= Features::DirectControl;
    }
    else
    {
        _processor->_features &= ~Features::DirectControl;
    }
    emit componentConfigurationChanged(component());
}

//  End of emuone-360/Ibm360ProcessorEditor.cpp
