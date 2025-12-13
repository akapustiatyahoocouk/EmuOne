//
//  emuone-util/Exceptions.cpp - emuone::util exeptions
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
#include "emuone-util/API.hpp"
using namespace emuone::util;

//////////
//  ParseException
ParseException::ParseException(
        const QString & string,
        qsizetype position
    ) : _string(string),
        _position(position)
{
}

QString ParseException::errorMessage() const
{
    return "Parse error in '" + _string +
           "' at position " + toString(_position);
}

//////////
//  NotImplementedError
NotImplementedError::NotImplementedError()
{
#ifdef Q_DEBUG
    #if defined(Q_OS_WINDOWS)
        __debugbreak();
    #elif defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
        raise(SIGTRAP)
    #else
        #error Unsupported host OS
    #endif
#endif
}

QString NotImplementedError::errorMessage() const
{
    return "Not yet implemented";
}

//  End of emuone-util/Exceptions.cpp
