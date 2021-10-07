//
//  emuone/VirtualApplianceWindow.cpp
//
//  VA UI frame
//
//////////
#include "emuone/API.hpp"
#include "ui_VirtualApplianceWindow.h"

//////////
//  Construction/destruction
VirtualApplianceWindow::VirtualApplianceWindow(core::VirtualAppliance * virtualAppliance, QWidget * parent)
    :   QMainWindow(parent),
        ui(new Ui::VirtualApplianceWindow),
        _virtualAppliance(virtualAppliance)
{
    ui->setupUi(this);
    Q_ASSERT(_virtualAppliance != nullptr);

    this->setWindowTitle(virtualAppliance->getName());
    this->setWindowIcon(virtualAppliance->getArchitecture()->getLargeIcon());
    this->setMinimumSize(320, 200);

    //  Create component UIs
    for (core::Component * component : virtualAppliance->getComponents())
    {
        if (core::ComponentUi * componentUi = component->createUi())
        {
            _uiMap.insert(component, componentUi);
        }
    }
    for (core::Adaptor * adaptor : virtualAppliance->getAdaptors())
    {
        if (core::ComponentUi * componentUi = adaptor->createUi())
        {
            _uiMap.insert(adaptor, componentUi);
        }
    }

    //  Create full-screen widgets
    core::FullScreenWidgetList allFullScreenWidgets;
    for (core::ComponentUi * componentUi : _uiMap)
    {
        core::FullScreenWidgetList fullScreenWidgets = componentUi->getFullScreenWidgets();
        allFullScreenWidgets.append(fullScreenWidgets);
    }
    if (allFullScreenWidgets.size() == 1)
    {   //  Can place the only FullScreenWidget directly into this frame
        allFullScreenWidgets[0]->setParent(this);
        _fullScreenWidget = allFullScreenWidgets[0];
    }
    else if (allFullScreenWidgets.size() > 1)
    {   //  Need a tab control, with one page per FullScreenWidget
        Q_ASSERT(false);
    }

    //  Set up custom event handlers
    _resizeControls();
}

VirtualApplianceWindow::~VirtualApplianceWindow()
{
    delete ui;
}

//////////
//  QWidget
void VirtualApplianceWindow::closeEvent(QCloseEvent * event)
{
    _virtualAppliance->stop();
    event->ignore();
}

void VirtualApplianceWindow::resizeEvent(QResizeEvent * /*event*/)
{
    _resizeControls();
}

//////////
//  Implementation helpers
void VirtualApplianceWindow::_resizeControls()
{
    QRect rc1 = centralWidget()->geometry();
    QRect rc2 = this->geometry();
    QRect rc3 = ui->menubar->geometry();
    QRect rc4 = ui->statusbar->geometry();
    rc1 = rc1;
    _fullScreenWidget->setGeometry(rc1.x(), rc3.height() / 2, rc1.width(), rc2.height() - rc3.height() - rc4.height());
}

//////////
//  Event handlers
void VirtualApplianceWindow::_actionStopTriggered()
{
    _virtualAppliance->stop();
}

void VirtualApplianceWindow::_actionSuspendTriggered()
{
    try
    {
        _virtualAppliance->suspend();
    }
    catch (core::VirtualApplianceException & ex)
    {
        QMessageBox msgBox;
        msgBox.setText(ex.getMessage());
        msgBox.exec();
    }
}

//  End of emuone/VirtualApplianceWindow.cpp
