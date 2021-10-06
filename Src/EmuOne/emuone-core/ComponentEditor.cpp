//
//  emuone-core/ComponentEditor.cpp
//
//  The ComponentEditor class implementation
//
//////////
#include "emuone-core/API.hpp"

//////////
//  Construction/destruction
ComponentEditor::ComponentEditor(Component * component, QWidget * parent)
    :   QWidget(parent),
        _component(component)
{
    Q_ASSERT(_component != nullptr);
}

//  End of emuone-core/ComponentEditor.cpp
