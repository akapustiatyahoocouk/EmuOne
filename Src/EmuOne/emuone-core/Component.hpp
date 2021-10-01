//
//  emuone-core/Component.hpp
//
//  A generic VM Component
//
//////////
#pragma once

//////////
//  A generic VM component
class EMUONE_CORE_EXPORT Component
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Component)

    //////////
    //  Construction/destruction
public:
    //  Constructs the component.
    //  Throws "VirtualApplianceException" if the new "name" is not a valid component name.
    explicit Component(const QString & name);
    virtual ~Component();

    //////////
    //  Operations
public:
    static bool         isValidName(const QString & name);

    //  Returns the name of this component.
    QString             getName() const { return _name; }

    //  Sets the name of this component.
    //  Throws "VirtualApplianceException" if the new "name" is not a valid component name.
    void                setName(const QString & name);

    //////////
    //  Implementation
private:
    QString             _name;
    VirtualAppliance *  _virtualAppliance = nullptr;    //  ...to which this component belongs; null == nojne
};

//  End of emuone-core/Component.hpp
