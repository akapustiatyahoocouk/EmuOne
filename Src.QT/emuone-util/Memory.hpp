//
//  emuone-util/Memory.hpp - low-level memory access
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

namespace emuone::util
{
    /// \brief
    ///     The order in which bytes of a multibyte
    ///     valye appear in memory.
    enum class ByteOrder
    {
        BigEndian = 0,      ///< MSB first (0x1234 -> 0x12, 0x34).
        LittleEndian = 1,   ///< LSB first (0x1234 -> 0x34, 0x12).
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
        Host = BigEndian
#elif Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        Host = LittleEndian
#else
    #error Unsupported byte order
#endif
    };
}

//  End of emuone-util/Memory.hpp
