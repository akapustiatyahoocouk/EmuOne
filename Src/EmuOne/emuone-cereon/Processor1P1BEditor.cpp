//
//  emuone-cereon/Processor1P1BEditor.cpp
//
//  cereon::Processor1P1BEditor class implementation
//
//////////
#include "emuone-cereon/API.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-promo"
#include "ui_Processor1P1BEditor.h"
#pragma GCC diagnostic pop
using namespace cereon;

//////////
//  Construction/destruction
Processor1P1BEditor::Processor1P1BEditor(Processor1P1B * processor, QWidget * parent)
    :   core::ComponentEditor(processor, parent),
        _ui(new Ui::Processor1P1BEditor),
        _processor(processor)
{
    Q_ASSERT(_processor != nullptr);

    _ui->setupUi(this);

    _refreshUnderway = true;
    _ui->_clockFrequencyUnitComboBox->addItem("Hz");
    _ui->_clockFrequencyUnitComboBox->addItem("KHz");
    _ui->_clockFrequencyUnitComboBox->addItem("MHz");
    _ui->_clockFrequencyUnitComboBox->addItem("GHz");
    _ui->_byteOrderComboBox->addItem("Big-endian");
    _ui->_byteOrderComboBox->addItem("Little-endian");
    _refreshUnderway = false;
}

Processor1P1BEditor::~Processor1P1BEditor()
{
    delete _ui;
}

//////////
//  core::ComponentEditor
void Processor1P1BEditor::refresh()
{
    _refreshUnderway = true;

    _ui->_clockFrequencyValueLineEdit->setText(util::toString(_processor->clockFrequency().numberOfUnits()));
    _ui->_clockFrequencyUnitComboBox->setCurrentIndex(static_cast<int>(_processor->clockFrequency().unit()));
    _ui->_byteOrderComboBox->setCurrentIndex(static_cast<int>(_processor->byteOrder()));

    _ui->_baseCheckBox->setChecked((_processor->features() & Features::Base) != Features::None);
    _ui->_floatingPointCheckBox->setChecked((_processor->features() & Features::FloatingPoint) != Features::None);
    _ui->_debugCheckBox->setChecked((_processor->features() & Features::Debug) != Features::None);
    _ui->_unalignedOperandsCheckBox->setChecked((_processor->features() & Features::UnalignedOperands) != Features::None);
    _ui->_protectedMemoryCheckBox->setChecked((_processor->features() & Features::ProtectedMemory) != Features::None);
    _ui->_virtualMemoryCheckBox->setChecked((_processor->features() & Features::VirtualMemory) != Features::None);
    _ui->_performanceMonitoringCheckBox->setChecked((_processor->features() & Features::PerformanceMonitoring) != Features::None);

    _ui->_processorIdLineEdit->setText(util::toString(_processor->processorId(), "%02X"));
    _ui->_primaryCheckBox->setChecked(_processor->primary());
    _ui->_bootstrapIpLineEdit->setText(util::toString(_processor->bootstrapIp(), "%016X"));

    _refreshUnderway = false;
}

//////////
//  Implementation  elpers
void Processor1P1BEditor::_applyClockCurrencyChanges()
{
    uint64_t numberOfUnits = _ui->_clockFrequencyValueLineEdit->text().toULongLong();
    core::ClockFrequency::Unit unit = static_cast<core::ClockFrequency::Unit>(_ui->_clockFrequencyUnitComboBox->currentIndex());
    if (numberOfUnits > 0 &&
        numberOfUnits * core::getUnitValue(unit) / core::getUnitValue(unit) == numberOfUnits)
    {   //  Parsed successfully AND no overflow
        _processor->setClockFrequency(core::ClockFrequency(unit, numberOfUnits));
    }
    emit componentConfigurationChanged(component());
}

void Processor1P1BEditor::_applyByteOrderChanges()
{
    util::ByteOrder byteOrder = static_cast<util::ByteOrder>(_ui->_byteOrderComboBox->currentIndex());
    _processor->setByteOrder(byteOrder);
    emit componentConfigurationChanged(component());
}

void Processor1P1BEditor::_applyProcessorIdChanges()
{
    uint8_t processorId = 0;
    if (util::fromString(_ui->_processorIdLineEdit->text(), "%x", processorId))
    {   // Use it
        _processor->setProcessorId(processorId);
        emit componentConfigurationChanged(component());
    }
}

void Processor1P1BEditor::_applyIsPrimaryChanges()
{
    _processor->setPrimary(_ui->_primaryCheckBox->isChecked());
    emit componentConfigurationChanged(component());
}

void Processor1P1BEditor::_applyBootstrapIpChanges()
{
    uint64_t bootstrapIp = 0;
    if (util::fromString(_ui->_bootstrapIpLineEdit->text(), "%llx", bootstrapIp))
    {   //  Use it
        _processor->setBootstrapIp(bootstrapIp);
        emit componentConfigurationChanged(component());
    }
}

//////////
//  Event handlers
void Processor1P1BEditor::_clockFrequencyValueLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applyClockCurrencyChanges();
    }
}

void Processor1P1BEditor::_clockFrequencyUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        _applyClockCurrencyChanges();
    }
}

void Processor1P1BEditor::_byteOrderComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        _applyByteOrderChanges();
    }
}

void Processor1P1BEditor::_processorIdLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applyProcessorIdChanges();
    }
}

void Processor1P1BEditor::_primaryCheckBoxStateChanged(int)
{
    if (!_refreshUnderway)
    {
        _applyIsPrimaryChanges();
    }
}

void Processor1P1BEditor::_bootstrapIpLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        _applyBootstrapIpChanges();
    }
}

//  End of emuone-cereon/Processor1P1BEditor.cpp
