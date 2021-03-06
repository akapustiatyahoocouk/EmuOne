//
//  emuone-core/Template.hpp
//
//  The VA templates
//
//////////
#pragma once

namespace core
{
    //////////
    //  Represents a generic VA template
    class EMUONE_CORE_EXPORT VirtualApplianceTemplate : public util::StockObject
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(VirtualApplianceTemplate)

        //////////
        //  Construction/destruction
    public:
        VirtualApplianceTemplate() {}
        virtual ~VirtualApplianceTemplate() {}

        //////////
        //  util::tockObject
    public:
        virtual QIcon       smallIcon() const override { return architecture()->smallIcon(); }
        virtual QIcon       largeIcon() const override { return architecture()->largeIcon(); }

        //////////
        //  Operations
    public:
        //  The Architecture to which this VA template belongs.
        virtual Architecture *  architecture() const = 0;

        //  Populates the specified "virtualAppliance" with Components as per this template
        //  Throws VirtualApplianceException if an error occurs.
        virtual void        populateVirtualAppliance(VirtualAppliance * virtualAppliance) = 0;
    };

    //////////
    //  Represents a generic VM template
    class EMUONE_CORE_EXPORT VirtualMachineTemplate : public VirtualApplianceTemplate
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(VirtualMachineTemplate)

        //////////
        //  Construction/destruction
    public:
        VirtualMachineTemplate() {}
        virtual ~VirtualMachineTemplate() {}
    };

    //////////
    //  Represents a generic RT template
    class EMUONE_CORE_EXPORT RemoteTerminalTemplate : public VirtualApplianceTemplate
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RemoteTerminalTemplate)

        //////////
        //  Construction/destruction
    public:
        RemoteTerminalTemplate() {}
        virtual ~RemoteTerminalTemplate() {}
    };
}

//  End of emuone-core/Template.hpp
