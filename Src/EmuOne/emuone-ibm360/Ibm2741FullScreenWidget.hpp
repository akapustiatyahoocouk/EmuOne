//
//  emuone-ibm360/Ibm2741FullScreenWidget.hpp
//
//  The full-screen widget of the IBM 2741 UI
//
//////////
#pragma once
#include "emuone-ibm360/API.hpp"
#include <QWidget>

using namespace ibm360;

namespace Ui { class Ibm2741FullScreenWidget; }

//////////
//  The full-screen widget of the IBM 2741 UI
class Ibm2741FullScreenWidget : public core::FullScreenWidget
{
    Q_OBJECT

    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Ibm2741FullScreenWidget)

    //////////
    //  Construction/destruction
public:
    explicit Ibm2741FullScreenWidget(Ibm2741 * ibm2741);
    virtual ~Ibm2741FullScreenWidget();

    //////////
    //  QWidget
protected:
    virtual void        paintEvent(QPaintEvent * event) override;

    //////////
    //  core::FullScreenWidget
public:
    virtual core::Component *   component() override;
    virtual QString     displayName() override;

    //////////
    //  Implementation
private:
    Ui::Ibm2741FullScreenWidget *   _ui;
    Ibm2741 *const      _ibm2741;
};

//  Endof emuone-ibm360/Ibm2741FullScreenWidget.hpp
