//
//  emuone-util/StockObject.cpp - Stock objects support
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
    /// \class StockObject emuone-util/API.hpp
    /// \brief An object that comes from a fixed "stock".
    class EMUONE_UTIL_PUBLIC IStockObject
    {
        EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IStockObject)

        //////////
        //  Construction/destruction
    protected:
        IStockObject() = default;
        virtual ~IStockObject() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the mnemonic identifier of this stock object.
        /// \return
        ///     The mnemonic identifier of this stock object.
        virtual QString mnemonic() const = 0;

        /// \brief
        ///     Returns the user-readable display name of this stock object.
        /// \return
        ///     The user-readable display name of this stock object.
        virtual QString displayName() const = 0;

        /// \brief
        ///     Returns the small (16x16) image reprsewnting this stock object.
        /// \return
        ///     The small (16x16) image reprsewnting this stock object.
        virtual QImage  smallImage() const = 0;

        /// \brief
        ///     Returns the large (32x32) image reprsewnting this stock object.
        /// \return
        ///     The large (32x32) image reprsewnting this stock object.
        virtual QImage  largeImage() const = 0;
    };
}

//  End of emuone-util/StockObject.cpp
