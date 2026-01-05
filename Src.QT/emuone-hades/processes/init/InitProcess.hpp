//
//  emuone-hades/processes/init/InitRunner.hpp - The "init" process
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

namespace emuone::hades::processes::init
{
    /// \class InitProcess emuone-hades/API.hpp
    /// \brief The "init" process.
    class EMUONE_HADES_PUBLIC InitProcess final
    {
        EMUONE_UTILITY_CLASS(InitProcess)

        //////////
        //  Constants
    public:
        inline static const QString ProcessName = "init";
        inline static const QString VirtuaPath = "SYSTEM:/bin/init";
        inline static const QString CurrentDirectory = "SYSTEM:/";

        //////////
        //  Types
    public:
        /// \class InitRunner emuone-hades/API.hpp
        /// \brief The runner for the "init"'s main thread.
        class EMUONE_HADES_PUBLIC Runner final
            :   public emuone::hades::kernel::NativeThreadRunner
        {
            EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Runner)

            //////////
            //  Construction/destruction
        public:
            Runner();
            virtual ~Runner();

            //////////
            //  emuone::hades::kernel::NativeThreadRunner
        protected:
            virtual uint32_t    run() override;
        };
    };
}

//  End of emuone-hades/processes/init/InitRunner.hpp
