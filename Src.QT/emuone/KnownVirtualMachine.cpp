//
//  emuone/KnownVirtualMachine.cpp - emuone::KnownVirtualMachine class implementation
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
#include "emuone/API.hpp"
using namespace emuone;

//////////
//  Construction/destruction
KnownVirtualMachine::KnownVirtualMachine(
        emuone::core::IArchitecture * architecture,
        emuone::core::IVirtualMachineType * type,
        const QString & name,
        const QString & location
    ) : _architecture(architecture),
        _type(type),
        _name(name),
        _location(location)
{
    Q_ASSERT(_architecture != nullptr);
    Q_ASSERT(_type != nullptr);
}

//////////
//  Operators
bool KnownVirtualMachine::operator == (const KnownVirtualMachine & op2) const
{
    return _name == op2._name && _location == op2._location;
}

bool KnownVirtualMachine::operator != (const KnownVirtualMachine & op2) const
{
    return !(*this == op2);
}

//////////
//  Formatting/parsing
namespace
{
    QString escape(const QString & s)
    {
        char escape[8];

        QString result;
        for (QChar c : s)
        {
            switch (c.unicode())
            {
                //  special characters
            case '\a':
                result += "\\a";
                break;
            case '\b':
                result += "\\b";
                break;
            case '\f':
                result += "\\f";
                break;
            case '\n':
                result += "\\n";
                break;
            case '\r':
                result += "\\r";
                break;
            case '\t':
                result += "\\t";
                break;
            case '\v':
                result += "\\v";
                break;
                //  literal escapes
            case '<':   case '>':
            case '[':   case ']':
            case ',':   case '\\':
                result += '\\';
                result += c;
                break;
                //  default case
            default:
                if (c.unicode() > 255)
                {   //  0uXXXX
                    sprintf(escape, "\\u%04X", c.unicode());
                    result += escape;
                }
                else if (c < '\x20' || c >= '\x7F')
                {   //  \xXX
                    sprintf(escape, "\\x%02X", c.unicode());
                    result += escape;
                }
                else
                {
                    result += c;
                }
                break;
            }
        }
        return result;
    }

    int xdigit(const QChar & c)
    {
        if (c >= '0' && c <= '9')
        {
            return c.unicode() - '0';
        }
        else if (c >= 'a' && c <= 'f')
        {
            return c.unicode() - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F')
        {
            return c.unicode() - 'A' + 10;
        }
        else
        {
            return -1;
        }
    }

    QString unescape(const QString & s)
    {   //   throws emuone::util::ParseException
        QString result;
        for (int i = 0; i < s.length(); )
        {
            if (s[i] != '\\')
            {   //  A literal character
                result += s[i];
                i++;
                continue;
            }
            //  We have an escape sequence. Skip '\'
            i++;
            if (i >= s.length())
            {   //  OOPS! Missing
                throw emuone::util::ParseException(s, 0);
            }
            //  Special character ?
            if (s[i] == 'a')
            {   //  \a
                result += '\a';
                i++;
            }
            else if (s[i] == 'b')
            {   //  \b
                result += '\b';
                i++;
            }
            else if (s[i] == 'f')
            {   //  \f
                result += '\f';
                i++;
            }
            else if (s[i] == 'n')
            {   //  \n
                result += '\n';
                i++;
            }
            else if (s[i] == 'r')
            {   //  \r
                result += '\r';
                i++;
            }
            else if (s[i] == 't')
            {   //  \t
                result += '\t';
                i++;
            }
            else if (s[i] == 'v')
            {   //  \a
                result += '\v';
                i++;
            }
            //  Numeric escape ?
            else if (s[i] == 'x')
            {   //  \xXX
                i++;
                if (i + 1 < s.length() &&
                    xdigit(s[i]) != -1 && xdigit(s[i + 1]) != -1)
                {
                    result += QChar(xdigit(s[i]) * 16 +
                                    xdigit(s[i + 1]));
                    i += 2;
                }
                else
                {   //  OOPS! Invalid hex escape!
                    throw emuone::util::ParseException(s, 0);
                }
            }
            else if (s[i] == 'u')
            {   //  \uXXXX
                i++;
                if (i + 3 < s.length() &&
                    xdigit(s[i]) != -1 && xdigit(s[i + 1]) != -1 &&
                    xdigit(s[i + 2]) != -1 && xdigit(s[i + 3]) != -1)
                {
                    result += QChar(xdigit(s[i]) * 4096 +
                                    xdigit(s[i + 1]) * 256 +
                                    xdigit(s[i + 2]) * 16 +
                                    xdigit(s[i + 3]));
                    i += 4;
                }
                else
                {   //  OOPS! Invalid hex escape!
                    throw emuone::util::ParseException(s, 0);
                }
            }
            //  Literal escape
            else
            {
                result += s[i++];
            }
        }
        return result;
    }
}
namespace emuone::util
{
    template <>
    QString toString<KnownVirtualMachine>(
            const KnownVirtualMachine & value
        )
    {
        return '<' +
               escape(value.architecture()->mnemonic()) +
               ',' +
               escape(value.type()->mnemonic()) +
               ',' +
               escape(value.name()) +
               ',' +
               escape(value.location()) +
               '>';
    }

    template <>
    QString toString<KnownVirtualMachines>(
            const KnownVirtualMachines & value
        )
    {
        QString result;
        result += '[';
        for (int i = 0; i < value.size(); i++)
        {
            result += toString(value[i]);
            if (i + 1 < value.size())
            {
                result += ',';
            }
        }
        result += ']';
        return result;
    }

    template <>
    KnownVirtualMachine fromString<KnownVirtualMachine>(
            const QString & s, qsizetype & scan
        )
    {
        //  Skip '<'
        if (scan >= s.length() || s[scan] != '<')
        {
            throw emuone::util::ParseException(s, scan);
        }
        qsizetype prescan;
        //  Find '>'
        for (prescan = scan + 1; prescan < s.length() && s[prescan] != '>'; prescan++)
        {   //  All work done in the loop header
        }
        if (prescan == s.length())
        {
            throw emuone::util::ParseException(s, scan);
        }
        //  Break into <mnemonic>,<address> parts
        QStringList chunks = s.mid(scan + 1, prescan - scan - 1).split(',');
        if (chunks.size() != 4)
        {
            throw emuone::util::ParseException(s, scan);
        }
        auto architecture =
            emuone::core::ArchitectureManager::find(chunks[0]);
        auto type =
            emuone::core::VirtualMachineTypeManager::find(chunks[1]);
        if (architecture == nullptr || type == nullptr)
        {
            throw emuone::util::ParseException(s, scan);
        }
        //  Resolve mnemonic
        auto result = KnownVirtualMachine(architecture, type, chunks[2], chunks[3]);
        scan = prescan + 1;
        return result;
    }

    template <>
    KnownVirtualMachines fromString<KnownVirtualMachines>(
            const QString & s, qsizetype & scan
        )
    {
        //  Skip '['
        if (scan >= s.length() || s[scan] != '[')
        {
            throw emuone::util::ParseException(s, scan);
        }
        qsizetype prescan = scan + 1;
        //  Parse list items
        KnownVirtualMachines result;
        if (prescan < s.length() && s[prescan] != ']')
        {   //  At least 1 item exists
            for (; ; )
            {
                KnownVirtualMachine workspaceAddress = fromString<KnownVirtualMachine>(s, prescan);   //  may throw
                if (!result.contains(workspaceAddress))
                {
                    result.append(workspaceAddress);
                }
                //  More ?
                if (prescan < s.length() && s[prescan] == ',')
                {   //  yes
                    prescan++;
                }
                else
                {   //  no
                    break;
                }
            }
        }
        //  Skip ']'
        if (prescan >= s.length() || s[prescan] != ']')
        {
            throw emuone::util::ParseException(s, scan);
        }
        //  Done
        scan = prescan + 1;
        return result;
    }
}

//  End of emuone/KnownVirtualMachine.cpp
