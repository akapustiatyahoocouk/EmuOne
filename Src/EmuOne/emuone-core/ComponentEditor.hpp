//
//  emuone-core/ComponentCategory.hpp
//
//  The ComponentCategory ADT
//
//////////
#pragma once
#include "emuone-core/API.hpp"
#include <QWidget>

//////////
//  A generic "editor" of a Component's properties
class EMUONE_CORE_EXPORT ComponentEditor : public QWidget
{
    Q_OBJECT

    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ComponentEditor)

    //////////
    //  Construction/destruction
public:
    ComponentEditor(Component * component, QWidget * parent = nullptr);

    //////////
    //  Operations
public:
    Component *     getComponent() const { return _component; }

    //  Adjusts all the editor's fields to reflect the current configuration
    //  of the edited component.
    virtual void    refresh() = 0;

signals:

    //////////
    //  Implementation
private:
    Component *     _component;
};

//  End of emuone-core/ComponentCategory.hpp

