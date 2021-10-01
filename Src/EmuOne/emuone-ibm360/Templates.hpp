//
//  emuone-ibm360/Templates.hpp
//
//  IBM/360 templates
//
//////////
#pragma once

class EMUONE_IBM360_EXPORT Ibm360H40VirtualMachineTemplate : public VirtualMachineTemplate
{
    DECLARE_SINGLETON(Ibm360H40VirtualMachineTemplate)

    //////////
    //  StockObject
public:
    virtual QString     getMnemonic() const override;
    virtual QString     getDisplayName() const override;

    //////////
    //  VirtualApplianceTemplate
public:
    virtual Architecture *  getArchitecture() const override;
};

//  End of emuone-ibm360/Templates.hpp
