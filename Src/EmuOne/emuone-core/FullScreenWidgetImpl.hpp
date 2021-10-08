//
//  emuone-core/FullScreenWidgetImpl.hpp
//
//  A full-screen widget of a component's UI
//
//////////
#pragma once
#include "emuone-core/API.hpp"
#include <QWidget>

//////////
//  A part of a Component's UI that will occupy the entire VA window.
//  It is initially created with a "hidden" frame as a "parent" and must be
//  re-parented to VA frame.
class EMUONE_CORE_EXPORT FullScreenWidgetImpl : public QWidget
{
    Q_OBJECT

    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(FullScreenWidgetImpl)

    //////////
    //  Construction/destruction
public:
    FullScreenWidgetImpl();
    virtual ~FullScreenWidgetImpl();

    //////////
    //  Operations
public:
    //  Returns the Component whose UI aspect this FullScreenWidget[Impl] represents.
    virtual core::Component *   component() = 0;

    //  Returns the short user-readable "display name" of this FullScreenWidget[Impl].
    virtual QString             displayName() = 0;
};

//  End of emuone-core/FullScreenWidgetImpl.hpp
