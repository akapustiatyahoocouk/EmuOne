//
//  emuone-360/Processor.cpp
//
//  ibm360::Processor class implementation
//
//////////
#include "emuone-ibm360/API.hpp"
using namespace ibm360;

//////////
//  Construction/destruction
Processor::Processor(const QString & name, Features features, const core::ClockFrequency & clockFrequency)
    :   Component(name),
        _features(normalise(features)),
        _clockFrequency(clockFrequency)
{
}

Processor::~Processor()
{
}

//////////
//  Component
core::ComponentType * Processor::type() const
{
    return Processor::Type::getInstance();
}

ComponentEditor * Processor::createEditor(QWidget * parent)
{
    return new Ibm360ProcessorEditor(this, parent);
}

QString Processor::shortStatus() const
{
    return "@" + _clockFrequency.toDisplayString();
}

//////////
//  Component (state control) - all thread-safe
core::Component::State Processor::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void Processor::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void Processor::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Processor::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void Processor::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void Processor::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }


    //  Done
    _state = State::Connected;
}

void Processor::disconnect() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Constructed;
}

//////////
//  Component (serialisation)
void Processor::serialiseConfiguration(QDomElement & configurationElement) const
{
    configurationElement.setAttribute("FetchProtection", ((_features & Features::FetchProtection) != Features::None) ? "yes" : "no");
    configurationElement.setAttribute("StoreProtection", ((_features & Features::StoreProtection) != Features::None) ? "yes" : "no");
    configurationElement.setAttribute("Decimal", ((_features & Features::Decimal) != Features::None) ? "yes" : "no");
    configurationElement.setAttribute("FloatingPoint", ((_features & Features::FloatingPoint) != Features::None) ? "yes" : "no");
    configurationElement.setAttribute("ByteOrientedOperand", ((_features & Features::ByteOrientedOperand) != Features::None) ? "yes" : "no");
    configurationElement.setAttribute("Timer", ((_features & Features::Timer) != Features::None) ? "yes" : "no");
    configurationElement.setAttribute("DirectControl", ((_features & Features::DirectControl) != Features::None) ? "yes" : "no");

    configurationElement.setAttribute("ClockFrequency", _clockFrequency.toString());

    //  TODO instruction timing model
}

void Processor::deserialiseConfiguration(QDomElement & configurationElement)
{
#define APPLY_FEATURE(Name)                                                         \
    QString feature##Name##String = configurationElement.attribute(#Name, "");      \
    if (feature##Name##String == "yes")                                             \
        _features |= Features::Name;                                                \
    else if (feature##Name##String == "no")                                         \
        _features &= ~Features::Name;

    APPLY_FEATURE(FetchProtection)
    APPLY_FEATURE(StoreProtection)
    APPLY_FEATURE(Decimal)
    APPLY_FEATURE(FloatingPoint)
    APPLY_FEATURE(ByteOrientedOperand)
    APPLY_FEATURE(Timer)
    APPLY_FEATURE(DirectControl)
#undef APPLY_FEATURE

    QString clockFrequencyString = configurationElement.attribute("ClockFrequency", _clockFrequency.toString());
    _clockFrequency = core::ClockFrequency::fromString(clockFrequencyString, _clockFrequency);
    //  TODO instruction timing model
}

//////////
//  Operations
bool Processor::isValidClockFrequency(const core::ClockFrequency & clockFrequency)
{
    return clockFrequency.toHz() > 0;
}

bool Processor::setFeatures(Features features)
{
    features = normalise(features);
    return true;
}

bool Processor::setClockFrequency(const core::ClockFrequency & clockFrequency)
{
    if (isValidClockFrequency(clockFrequency))
    {
        _clockFrequency = clockFrequency;
        return true;
    }
    return false;
}

core::ComponentUi * Processor::createUi()
{
    return nullptr;
}

//////////
//  Ibm360Processor::Type
IMPLEMENT_SINGLETON(Processor::Type)
Processor::Type::Type() {}
Processor::Type::~Type() {}

QString Processor::Type::mnemonic() const
{
    return "ibm360::Processor";
}

QString Processor::Type::displayName() const
{
    return "IBM/360 processor";
}

core::ComponentCategory * Processor::Type::category() const
{
    return core::StandardComponentCategories::Processors::getInstance();
}

bool Processor::Type::isCompatibleWith(core::Architecture * architecture) const
{
    return architecture == Architecture::getInstance();
}

Processor * Processor::Type::createComponent()
{
    return new Processor("Processor", Features::Default, core::ClockFrequency(core::ClockFrequency::Unit::MHZ, 2));
}

//  End of emuone-360/Processor.cpp
