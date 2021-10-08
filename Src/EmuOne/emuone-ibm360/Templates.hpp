//
//  emuone-ibm360/Templates.hpp
//
//  IBM/360 templates
//
//////////
#pragma once

namespace ibm360
{
    //////////
    //  The IBM/360 model H40 template
    class EMUONE_IBM360_EXPORT H40VirtualMachineTemplate : public core::VirtualMachineTemplate
    {
        DECLARE_SINGLETON(H40VirtualMachineTemplate)

        //////////
        //  StockObject
    public:
        virtual QString         mnemonic() const override;
        virtual QString         displayName() const override;

        //////////
        //  VirtualApplianceTemplate
    public:
        virtual Architecture *  architecture() const override;
        virtual void            populateVirtualAppliance(core::VirtualAppliance * virtualAppliance) override;
    };
}

//  End of emuone-ibm360/Templates.hpp
