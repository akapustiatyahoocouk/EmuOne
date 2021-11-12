//
//  emuone-cereon/Processor1P1BEditor.cpp
//
//  cereon::Processor1P1BEditor class implementation
//
//////////
#include "emuone-cereon/API.hpp"
#pragma GCC diagnostic push
#include "ui_Processor1P1BEditor.h"
#pragma GCC diagnostic pop
using namespace cereon;

//////////
//  Construction/destruction
Processor1P1BEditor::Processor1P1BEditor(Processor1P1B * processor, QWidget * parent)
    :   core::ComponentEditor(processor, parent),
        _ui(new Ui::Processor1P1BEditor),
        _processor(processor)
{
    Q_ASSERT(_processor != nullptr);

    _ui->setupUi(this);
}

Processor1P1BEditor::~Processor1P1BEditor()
{
    delete _ui;
}

//  End of emuone-cereon/Processor1P1BEditor.cpp

