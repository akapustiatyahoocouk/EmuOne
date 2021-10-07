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
    :   ui(new Ui::Ibm2741FullScreenWidget),
        _ibm2741(ibm2741)
{
    ui->setupUi(this);
    Q_ASSERT(_ibm2741 != nullptr);
}

Ibm2741FullScreenWidget::~Ibm2741FullScreenWidget()
{
    delete ui;
}


//////////
//  QWidget
void Ibm2741FullScreenWidget::paintEvent(QPaintEvent * event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rc = this->geometry();
    painter.fillRect(rc, QColor(255, 255, 255));
    painter.setPen(QColor(255, 0, 0));
    painter.drawRect(rc.x(), rc.y(), rc.width() - 30, rc.height() - 30);

    painter.end();
}

//  End of emuone-360/Ibm2741FullScreenWidget.cpp
