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
};

//  End of emuone-core/FullScreenWidgetImpl.hpp
