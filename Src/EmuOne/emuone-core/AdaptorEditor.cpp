//
//  emuone-core/AdaptorEditor.cpp
//
//  The AdaptorEditor class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  Construction/destruction
AdaptorEditor::AdaptorEditor(core::Adaptor * adaptor, QWidget * parent)
    :   QWidget(parent),
        _adaptor(adaptor)
{
    Q_ASSERT(_adaptor != nullptr);
}

//  End of emuone-core/AdaptorEditor.cpp
