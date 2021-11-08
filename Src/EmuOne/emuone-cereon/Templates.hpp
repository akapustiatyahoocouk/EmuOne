//
//  emuone-cereon/Templates.hpp
//
//  Cereon-based PC templates
//
//////////
#pragma once

namespace cereon
{
    //////////
    //  The Cereon Workstation Mini template
    class EMUONE_CEREON_EXPORT CereonWorkstationMiniTemplate : public core::VirtualMachineTemplate
    {
        DECLARE_SINGLETON(CereonWorkstationMiniTemplate)

        //////////
        //  StockObject
    public:
        virtual QString         mnemonic() const override;
        virtual QString         displayName() const override;

        //////////
        //  VirtualApplianceTemplate
    public:
        virtual CereonWorkstationArchitecture * architecture() const override;
        virtual void            populateVirtualAppliance(core::VirtualAppliance * virtualAppliance) override;
    };
}

//  End of emuone-cereon/Templates.hpp
