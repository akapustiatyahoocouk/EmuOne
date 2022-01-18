//
//  emuone-hadesos/Validation.cpp
//
//  The hadesos::Validation class implementation
//
//////////
#include "emuone-hadesos/API.hpp"
using namespace hadesos;

//////////
//  Operations
bool Validation::isValidVolumeName(const QString & volumeName)
{
    if (volumeName.length() == 0 || volumeName.length() > 128 || volumeName.trimmed().length() != volumeName.length())
    {   //  OOPS! Invalid format!
        return false;
    }
    for (QChar c : volumeName)
    {
        if (!c.isLetter() && !c.isDigit() && (c != '_' && c != '.'))
        {   //  OOPS! Invalid content!
            return false;
        }
    }
    return true;    //  all checks passed
}

//  End of emuone-hadesos/Validation.cpp
