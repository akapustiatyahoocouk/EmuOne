//
//  emuone-core/ComponentEditor.cpp
//
//  The ComponentEditor class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  Construction/destruction
ComponentEditor::ComponentEditor(core::Component * component, QWidget * parent)
    :   QWidget(parent),
        _component(component)
{
    Q_ASSERT(_component != nullptr);
}

//  End of emuone-core/ComponentEditor.cpp
