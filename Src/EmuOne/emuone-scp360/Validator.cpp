//
//  emuone-scp360/Validator.cpp
//
//  The scp360::Validator class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Operations
bool Validator::isValidSegmentName(const QString & name)
{
    if (name.isEmpty())
    {   //  Anonymous segment
        return true;
    }
    if (name.length() > 8)
    {   //  OOPS! Too long!
        return false;
    }
    for (int i = 0; i < name.length(); i++)
    {
        QChar ch = name[i];
        if (!((ch >= 'A' && ch < 'Z') || (ch >= '0' && ch <= '9')))
        {   //  OOPS! Not allowed in segment names
            return false;
        }
    }
    return true;
}

bool Validator::isValidProcessName(const QString & name)
{
    if (name.length() == 0 || name.length() > 16)
    {   //  OOPS! Empty or too long!
        return false;
    }
    for (int i = 0; i < name.length(); i++)
    {
        QChar ch = name[i];
        if (!((ch >= 'A' && ch < 'Z') || (ch >= '0' && ch <= '9')))
        {   //  OOPS! Not allowed in process names
            return false;
        }
    }
    return true;
}

bool Validator::isValidEnvironmentVariableName(const QString & name)
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

bool Validator::isValidEnvironmentVariableScalarValue(const QString & scalarValue)
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

bool Validator::isValidEnvironmentVariableListValue(const QStringList & listValue)
{
    if (listValue.size() == 0)
    {
        return false;
    }
    for (int i = 0; i < listValue.size(); i++)
    {
        if (!isValidEnvironmentVariableScalarValue(listValue[i]))
        {
            return false;
        }
    }
    return true;
}

bool Validator::isValidPhysicalDeviceName(const QString & name)
{
    return name.length() == 4 &&
           name[0] == '#' &&
           (name[1] >= '0' && name[0] <= '6') &&
           ((name[2] >= '0' && name[2] <= '9') || (name[2] >= 'A' && name[2] <= 'F')) &&
           ((name[3] >= '0' && name[3] <= '9') || (name[3] >= 'A' && name[3] <= 'F'));
}

bool Validator::isValidLogicalDeviceName(const QString & name)
{
    return false;   //  TODO implerment properly
}

bool Validator::isValidVolumeName(const QString & name)
{
    if (name.length() == 0 || name.length() > 8)
    {   //  OOPS! Empty or Too long!
        return false;
    }
    for (int i = 0; i < name.length(); i++)
    {
        QChar ch = name[i];
        if (!((ch >= 'A' && ch < 'Z') || (ch >= '0' && ch <= '9') || (ch == '.')))
        {   //  OOPS! Not allowed in volume names
            return false;
        }
    }
    if (name.startsWith(".") || name.endsWith(".") || name.contains(".."))
    {   //  OOPS! Invalid syntax!
        return false;
    }
    return true;
}

//  End of emuone-scp360/Validator.cpp
