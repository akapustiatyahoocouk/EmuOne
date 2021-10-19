//
//  emuone-scp360/Process.cpp
//
//  The scp360::Process class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
Process::Process(Scp * scp, uint16_t id, const QString & name, Flags flags, Process * parent)
    :   Object(scp, id),
        _name(name),
        _flags(flags),
        _parent(parent)
{
    Q_ASSERT(isValidName(_name));
    Q_ASSERT(_parent == nullptr || _parent->scp() == scp);

    if (_parent != nullptr)
    {
        _parent->_children.append(this);
        for (EnvironmentVariable * parentEnvironmentVariable : _parent->_environment)
        {
            _environment.insert(parentEnvironmentVariable->name(), parentEnvironmentVariable->clone());
        }
        _staticPriority = _parent->_staticPriority;
        _dynamicPriority = 0;
    }
    else
    {
        _staticPriority = DefaultStaticPriority;
        _dynamicPriority = 0;
    }
}

Process::~Process()
{
    if (_parent != nullptr)
    {
        Q_ASSERT(_parent->_children.contains(this));
        _parent->_children.removeOne(this);
    }
}

//////////
//  Operations
bool Process::isValidName(const QString & name)
{
    if (name.length() == 0 || name.length() > 16)
    {   //  OOPS! Empty or too long!
        return false;
    }
    for (int i = 0; i < name.length(); i++)
    {
        QChar ch = name[i];
        if (!((ch >= 'A' && ch < 'Z') || (ch >= '0' || ch <= '9')))
        {   //  OOPS! Not allowed in process names
            return false;
        }
    }
    return true;
}

void Process::setState(State state)
{
    Q_ASSERT(scp()->onKernelThread());

    _state = state;
}

bool Process::hasEnvironmentVariable(const QString & name) const
{
    Q_ASSERT(scp()->onKernelThread());

    return _environment.contains(name);
}

ErrorCode Process::getEnvironmentVariableValueCount(const QString & name, int & value) const
{
    Q_ASSERT(scp()->onKernelThread());

    if (_environment.contains(name))
    {
        value = _environment[name]->valueCount();
        return ErrorCode::ERR_OK;
    }
    else
    {
        return ErrorCode::ERR_NOF;
    }
}

ErrorCode Process::getEnvironmentVariableValueScalar(const QString & name, QString & value) const
{
    Q_ASSERT(scp()->onKernelThread());

    if (_environment.contains(name))
    {
        QStringList values = _environment[name]->values();
        value = (values.size() == 1) ? values[0] : ('(' + values.join(',') + ')');
        return ErrorCode::ERR_OK;
    }
    else
    {
        return ErrorCode::ERR_NOF;
    }
}

ErrorCode Process::setEnvironmentVariableValue(const QString & name, const QString & scalarValue)
{
    Q_ASSERT(scp()->onKernelThread());

    if (!EnvironmentVariable::isValidName(name) || !EnvironmentVariable::isValidScalarValue(scalarValue))
    {
        return ErrorCode::ERR_PAR;
    }
    //  Does the variable already exist ?
    if (_environment.contains(name))
    {   //  Yes, and it will be replaced
        delete _environment[name];
    }
    //  Create the variable
    _environment.insert(name, new EnvironmentVariable(name, scalarValue));
    return ErrorCode::ERR_OK;
}

ErrorCode Process::setEnvironmentVariableValue(const QString & name, const QStringList & listValue)
{
    Q_ASSERT(scp()->onKernelThread());

    if (!EnvironmentVariable::isValidName(name) || !EnvironmentVariable::isValidListValue(listValue))
    {
        return ErrorCode::ERR_PAR;
    }
    //  Does the variable already exist ?
    if (_environment.contains(name))
    {   //  Yes, and it will be replaced
        delete _environment[name];
    }
    //  Create the variable
    _environment.insert(name, new EnvironmentVariable(name, listValue));
    return ErrorCode::ERR_OK;
}

//////////
//  Helper functions
EMUONE_SCP360_EXPORT Process::Flags scp360::operator & (Process::Flags op1, Process::Flags op2)
{
    return Process::Flags(uint16_t(op1) & uint16_t(op2));
}

EMUONE_SCP360_EXPORT Process::Flags scp360::operator | (Process::Flags op1, Process::Flags op2)
{
    return Process::Flags(uint16_t(op1) | uint16_t(op2));
}

EMUONE_SCP360_EXPORT Process::Flags & scp360::operator &= (Process::Flags & op1, Process::Flags op2)
{
    op1 = op1 & op2;
    return op1;
}

EMUONE_SCP360_EXPORT Process::Flags & scp360::operator |= (Process::Flags & op1, Process::Flags op2)
{
    op1 = op1 | op2;
    return op1;
}

//  End of emuone-scp360/Process.cpp
