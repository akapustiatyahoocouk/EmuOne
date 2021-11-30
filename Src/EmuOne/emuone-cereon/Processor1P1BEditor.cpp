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

    _ui->_clockFrequencyValueLineEdit->setText(QString::number(_processor->clockFrequency().numberOfUnits()));
    _ui->_clockFrequencyUnitComboBox->setCurrentIndex(static_cast<int>(_processor->clockFrequency().unit()));
    _ui->_byteOrderComboBox->setCurrentIndex(static_cast<int>(_processor->byteOrder()));

    _ui->_baseCheckBox->setChecked((_processor->features() & Features::Base) != Features::None);
    _ui->_floatingPointCheckBox->setChecked((_processor->features() & Features::FloatingPoint) != Features::None);
    _ui->_debugCheckBox->setChecked((_processor->features() & Features::Debug) != Features::None);
    _ui->_unalignedOperandsCheckBox->setChecked((_processor->features() & Features::UnalignedOperands) != Features::None);
    _ui->_protectedMemoryCheckBox->setChecked((_processor->features() & Features::ProtectedMemory) != Features::None);
    _ui->_virtualMemoryCheckBox->setChecked((_processor->features() & Features::VirtualMemory) != Features::None);
    _ui->_performanceMonitoringCheckBox->setChecked((_processor->features() & Features::PerformanceMonitoring) != Features::None);

    _refreshUnderway = false;
}

//////////
//  Event handlers
void Processor1P1BEditor::_clockFrequencyValueLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        uint64_t numberOfUnits = _ui->_clockFrequencyValueLineEdit->text().toULongLong();
        core::ClockFrequency::Unit unit = static_cast<core::ClockFrequency::Unit>(_ui->_clockFrequencyUnitComboBox->currentIndex());
        if (numberOfUnits > 0)
        {
            _processor->setClockFrequency(core::ClockFrequency(unit, numberOfUnits));
        }
        emit componentConfigurationChanged(component());
    }
}

void Processor1P1BEditor::_clockFrequencyUnitComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        uint64_t numberOfUnits = _ui->_clockFrequencyValueLineEdit->text().toULongLong();
        core::ClockFrequency::Unit unit = static_cast<core::ClockFrequency::Unit>(_ui->_clockFrequencyUnitComboBox->currentIndex());
        if (numberOfUnits > 0)
        {
            _processor->setClockFrequency(core::ClockFrequency(unit, numberOfUnits));
        }
        emit componentConfigurationChanged(component());
    }
}

void Processor1P1BEditor::_byteOrderComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        util::ByteOrder byteOrder = static_cast<util::ByteOrder>(_ui->_byteOrderComboBox->currentIndex());
        _processor->setByteOrder(byteOrder);
        emit componentConfigurationChanged(component());
    }
}

//  End of emuone-cereon/Processor1P1BEditor.cpp
