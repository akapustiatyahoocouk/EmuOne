//
//  emuone-360/Ibm2741Editor.cpp
//
//  Ibm2741Editor class implementation
//
//////////
#include "emuone-ibm360/API.hpp"
#include "ui_Ibm2741Editor.h"
using namespace ibm360;

//////////
//  Construction/destruction
Ibm2741Editor::Ibm2741Editor(ibm360::Ibm2741 * ibm2741, QWidget * parent)
    :   ComponentEditor(ibm2741, parent),
        _ui(new Ui::Ibm2741Editor),
        _ibm2741(ibm2741)
{
    _ui->setupUi(this);
    Q_ASSERT(_ibm2741 != nullptr);
}

Ibm2741Editor::~Ibm2741Editor()
{
    _ibm2741->_editors.removeOne(this);
    delete _ui;
}

//////////
//  ComponentEditor
void Ibm2741Editor::refresh()
{
    _refreshUnderway = true;

    _ui->_addressLineEdit->setText(("000" + QString::number(_ibm2741->address(), 16)).right(3).toUpper());

    _refreshUnderway = false;
}

//////////
//  Event handlers
void Ibm2741Editor::_addressLineEditTextChanged(const QString &)
{
    if (!_refreshUnderway)
    {
        bool parsedOk = false;
        uint16_t address = (uint16_t)_ui->_addressLineEdit->text().toUInt(&parsedOk, 16);
        if (Device::isValidAddress(address))
        {
            _ibm2741->setAddress(address);
            emit componentConfigurationChanged(component());
        }
    }
}

//  End of emuone-360/Ibm2741Editor.cpp
