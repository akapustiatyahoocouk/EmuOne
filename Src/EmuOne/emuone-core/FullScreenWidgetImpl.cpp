//
//  emuone-core/FullScreenWidgetImpl.cpp
//
//  The FullScreenWidgetImpl class implementation
//
//////////
#include "emuone-core/API.hpp"
#include <QMainWindow>

namespace
{
    class HiddenParent : public QMainWindow
    {
        DECLARE_SINGLETON(HiddenParent)
    };

    IMPLEMENT_SINGLETON(HiddenParent)
    HiddenParent::HiddenParent() {}
    HiddenParent::~HiddenParent() {}
}

//////////
//  Construction/destruction
FullScreenWidgetImpl::FullScreenWidgetImpl()
    :   QWidget(HiddenParent::getInstance())
{
}

FullScreenWidgetImpl::~FullScreenWidgetImpl()
{
}

//  End of emuone-core/FullScreenWidgetImpl.cpp
