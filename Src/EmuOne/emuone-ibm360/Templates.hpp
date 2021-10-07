//
//  emuone-ibm360/Templates.hpp
//
//  IBM/360 templates
//
//////////
#pragma once

namespace ibm360
{
    class EMUONE_IBM360_EXPORT H40VirtualMachineTemplate : public core::VirtualMachineTemplate
    {
        DECLARE_SINGLETON(H40VirtualMachineTemplate)

        //////////
        //  StockObject
    public:
        virtual QString     getMnemonic() const override;
        virtual QString     getDisplayName() const override;

        //////////
        //  VirtualApplianceTemplate
    public:
        virtual Architecture *  getArchitecture() const override;
        virtual void        populateVirtualAppliance(core::VirtualAppliance * virtualAppliance) override;
    };
}

//  End of emuone-ibm360/Templates.hpp
