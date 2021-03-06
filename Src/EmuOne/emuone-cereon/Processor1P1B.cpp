//
//  emuone-cereon/Processor1P1B.cpp
//
//  cereon::Processor1P1B class implementation
//
//////////
#include "emuone-cereon/API.hpp"
using namespace cereon;

//////////
//  Construction/destruction
Processor1P1B::Processor1P1B(const QString & name, uint8_t processorId, bool primary, uint64_t bootstrapIp)
    :   Processor(name,
                  core::ClockFrequency(core::ClockFrequency::Unit::MHZ, 10),
                  util::ByteOrder::BigEndian,
                  processorId,
                  primary,
                  bootstrapIp),
      _core(this, Features::Base, 0, true)
{
    addCore(&_core);
}

Processor1P1B::~Processor1P1B()
{
}

//////////
//  core::Component
core::ComponentType * Processor1P1B::type() const
{
    return Type::instance();
}

core::ComponentEditor * Processor1P1B::createEditor(QWidget * parent)
{
    return new Processor1P1BEditor(this, parent);
}

QString Processor1P1B::shortStatus() const
{
    return "@" + clockFrequency().toDisplayString();
}

core::ComponentUi * Processor1P1B::createUi()
{
    return nullptr;
}

//////////
//  Processor1P1B::Type
IMPLEMENT_SINGLETON(Processor1P1B::Type)
Processor1P1B::Type::Type() {}
Processor1P1B::Type::~Type() {}

QString Processor1P1B::Type::mnemonic() const
{
    return "Cereon-1P1B";
}

QString Processor1P1B::Type::displayName() const
{
    return "Cereon-1P1B";
}

core::ComponentCategory * Processor1P1B::Type::category() const
{
    return core::StandardComponentCategories::Processors::instance();
}

bool Processor1P1B::Type::isCompatibleWith(const core::Architecture * architecture) const
{
    return dynamic_cast<const CereonArchitecture*>(architecture) != nullptr;
}

core::Component * Processor1P1B::Type::createComponent()
{
    return new Processor1P1B("Processor", 0, true, UINT64_C(0xFFFFFFFFFFF00000));
}

//  End of emuone-cereon/Processor1P1B.cpp
