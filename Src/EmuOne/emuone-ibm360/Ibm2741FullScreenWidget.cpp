//
//  emuone-360/Ibm2741FullScreenWidget.cpp
//
//  Ibm2741FullScreenWidget class implementation
//
//////////
#include "emuone-ibm360/API.hpp"
#include "ui_Ibm2741FullScreenWidget.h"
using namespace ibm360;

//////////
//  Construction/destruction
Ibm2741FullScreenWidget::Ibm2741FullScreenWidget(Ibm2741 * ibm2741)
    :   _ui(new Ui::Ibm2741FullScreenWidget),
        _ibm2741(ibm2741)
{
    _ui->setupUi(this);
    Q_ASSERT(_ibm2741 != nullptr);
}

Ibm2741FullScreenWidget::~Ibm2741FullScreenWidget()
{
    delete _ui;
}


//////////
//  QWidget
void Ibm2741FullScreenWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rc = this->geometry();
    painter.fillRect(rc, QColor(255, 255, 255));
    painter.setPen(QColor(255, 0, 0));
    painter.drawRect(rc.x(), rc.y(), rc.width(), rc.height());

    painter.end();
}

//////////
//  core::FullScreenWidget
core::Component * Ibm2741FullScreenWidget::component()
{
    return _ibm2741;
}

QString Ibm2741FullScreenWidget::displayName()
{
    return _ibm2741->name() + " " + _ibm2741->shortStatus();
}

//  End of emuone-360/Ibm2741FullScreenWidget.cpp
