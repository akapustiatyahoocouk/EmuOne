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
    :   _scp(scp),
        _id(id),
        _name(name),
        _flags(flags),
        _parent(parent)
{
    Q_ASSERT(_scp != nullptr);
    Q_ASSERT(_parent == nullptr || _parent->scp() == _scp);

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

//  End of emuone-scp360/Process.cpp
