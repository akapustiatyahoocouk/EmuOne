//
//  emuone-scp360/EnvironmentVariable.cpp
//
//  The scp360::EnvironmentVariable class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
EnvironmentVariable::EnvironmentVariable(const QString & name, const QString & scalarValue)
    :   _name(name)
{
    Q_ASSERT(Validator::isValidEnvironmentVariableScalarValue(scalarValue));
    _values.append(scalarValue);
}

EnvironmentVariable::EnvironmentVariable(const QString & name, const QStringList & listValue)
    :   _name(name),
        _values(listValue)
{
    Q_ASSERT(Validator::isValidEnvironmentVariableListValue(listValue));
}

//////////
//  Operations
QString EnvironmentVariable::toString() const
{
    if (_values.size() == 1)
    {   //  Already a scalar
        return _values[0];
    }
    else
    {   //  A list
        return "(" + _values.join(",") + ")";
    }
}

EnvironmentVariable * EnvironmentVariable::clone() const
{
    return new EnvironmentVariable(_name, _values);
}

//  End of emuone-scp360/EnvironmentVariable.cpp
