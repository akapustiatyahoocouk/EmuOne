//
//  emuone-hades/kernel/SharedFolder.cpp - emuone::hades::kernel::SharedFolder class implementation
//
//  EmuOne
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////
#include "emuone-hades/API.hpp"
using namespace emuone::hades::kernel;

//////////
//  Construction/destruction/assignment
SharedFolder::SharedFolder(const QString & volumeName, const QString & hostPath)
    :   _volumeName(volumeName),
        _hostPath(hostPath)
{
}

//////////
//  Operators
bool SharedFolder::operator == (const SharedFolder & op2) const
{
    return _volumeName == op2._volumeName &&
           _hostPath == op2._hostPath;
}

bool SharedFolder::operator != (const SharedFolder & op2) const
{
    return !(*this == op2);
}

bool SharedFolder::operator <  (const SharedFolder & op2) const
{
    return (_volumeName < op2._volumeName) ||
           (_volumeName == op2._volumeName &&
            _hostPath < op2._hostPath);
}

bool SharedFolder::operator <= (const SharedFolder & op2) const
{
    return !(*this > op2);
}

bool SharedFolder::operator >  (const SharedFolder & op2) const
{
    return op2 < *this;
}

bool SharedFolder::operator >= (const SharedFolder & op2) const
{
    return !(*this < op2);
}

//////////
//  Operations
bool SharedFolder::isValidVolumeName(const QString & volumeName)
{   //  TODO implement properly
    if (volumeName.trimmed().length() != volumeName.length() ||
        volumeName.length() == 0)
    {   //  OOPS!
        return false;
    }
    return true;
}

bool SharedFolder::isValidHostPath(const QString & hostPath)
{
    return QFileInfo(hostPath).isNativePath();
}

//  End of emuone-hades/kernel/SharedFolder.cpp
