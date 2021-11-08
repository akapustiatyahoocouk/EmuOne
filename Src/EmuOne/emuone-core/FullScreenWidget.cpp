//
//  emuone-core/FullScreenWidgetImpl.cpp
//
//  The FullScreenWidgetImpl class implementation
//
//////////
#include "emuone-core/API.hpp"
#include <QMainWindow>
using namespace core;

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
FullScreenWidget::FullScreenWidget()
    :   QWidget(HiddenParent::instance())
{
}

FullScreenWidget::~FullScreenWidget()
{
}

//  End of emuone-core/FullScreenWidgetImpl.cpp
