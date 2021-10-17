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
    Q_ASSERT(isValidScalarValue(scalarValue));
    _values.append(scalarValue);
}

EnvironmentVariable::EnvironmentVariable(const QString & name, const QStringList & listValue)
    :   _name(name),
        _values(listValue)
{
    Q_ASSERT(isValidListValue(listValue));
}

//////////
//  Operations
bool EnvironmentVariable::isValidName(const QString & name)
{
    if (name.length() < 1 || name.length() > 8)
    {
        return false;
    }
    for (int i = 0; i < name.length(); i++)
    {
        if (i == 0)
        {   //  Must be a letter
            if (!(name[i] >= 'A' && name[i] <= 'Z'))
            {
                return false;
            }
        }
        else
        {   //  Must be a letter or a digit
            if (!(name[i] >= 'A' && name[i] <= 'Z') && !(name[i] >= '0' && name[i] <= '9'))
            {
                return false;
            }
        }
    }
    return true;
}

bool EnvironmentVariable::isValidScalarValue(const QString & scalarValue)
{
    if (scalarValue.length() < 1 || scalarValue.length() > 128)
    {
        return false;
    }
    for (int i = 0; i < scalarValue.length(); i++)
    {
        if (scalarValue[i].unicode() < 32 || scalarValue[i].unicode() > 126 ||
            scalarValue[i].isSpace() || scalarValue[i] == ',' || scalarValue[i] == '(' || scalarValue[i] == ')')
        {
            return false;
        }
    }
    return true;
}

bool EnvironmentVariable::isValidListValue(const QStringList & listValue)
{
    if (listValue.size() == 0)
    {
        return false;
    }
    for (int i = 0; i < listValue.size(); i++)
    {
        if (!isValidScalarValue(listValue[i]))
        {
            return false;
        }
    }
    return true;
}

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
