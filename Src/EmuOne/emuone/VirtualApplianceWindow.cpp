//
//  emuone/VirtualApplianceWindow.cpp
//
//  VA UI frame
//
//////////
#include "emuone/API.hpp"

#include "VirtualApplianceWindow.hpp"
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
