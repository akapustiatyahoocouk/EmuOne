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
