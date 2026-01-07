//
//  emuone-hades/systemprocesses/DeviceManager.hpp - The "DeviceManager" process
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

namespace emuone::hades::systemprocesses
{
    /// \class DeviceManager emuone-hades/API.hpp
    /// \brief The "DeviceManager" process.
    class EMUONE_HADES_PUBLIC DeviceManager final
        :   public virtual ISystemProcess
    {
        EMUONE_DECLARE_SINGLETON(DeviceManager)

        //////////
        //  Constants
    public:
        inline static const QString ProcessName = "devmgr";
        inline static const QString VirtuaPath = "SYSTEM:/bin/devmgr";
        inline static const QString CurrentDirectory = "SYSTEM:/";

        inline static const QString ServerName = "System.DeviceManager";
        inline static const int ServerVersion = 1;

        //////////
        //  ISystemProcess
    public:
        virtual QString virtualPath() const override;
        virtual QString processName() const override;
        virtual Runner *createRunner() override;

        //////////
        //  Implementation
    private:
        class _Runner final
            :   public emuone::hades::kernel::NativeThreadRunner
        {
            EMUONE_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Runner)

            //////////
            //  Construction/destruction
        public:
            _Runner() = default;
            virtual ~_Runner() = default;

            //////////
            //  emuone::hades::kernel::NativeThreadRunner
        protected:
            virtual uint32_t    run() override;

            //////////
            //  Implementation
        private:
            //  Helpers
            void            _sighandler(int sig);
        };
    };
}

//  End of emuone-hades/systemprocesses/DeviceManager.hpp
