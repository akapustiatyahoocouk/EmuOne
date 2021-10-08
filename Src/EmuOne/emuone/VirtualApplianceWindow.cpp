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
        _ui(new Ui::VirtualApplianceWindow),
        _virtualAppliance(virtualAppliance)
{
    _ui->setupUi(this);
    Q_ASSERT(_virtualAppliance != nullptr);

    this->setWindowTitle(virtualAppliance->name());
    this->setWindowIcon(virtualAppliance->architecture()->largeIcon());
    this->setMinimumSize(320, 200);

    //  Create component UIs
    for (core::Component * component : virtualAppliance->components())
    {
        if (core::ComponentUi * componentUi = component->createUi())
        {
            _uiMap.insert(component, componentUi);
        }
    }
    for (core::Adaptor * adaptor : virtualAppliance->adaptors())
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
        core::FullScreenWidgetList fullScreenWidgets = componentUi->fullScreenWidgets();
        allFullScreenWidgets.append(fullScreenWidgets);
    }

    this->centralWidget()->setLayout(new QGridLayout());
    /*  TODO uncomment if (allFullScreenWidgets.size() == 1)
    {   //  Can place the only FullScreenWidget directly into this frame
        allFullScreenWidgets[0]->setParent(this->centralWidget());
        this->centralWidget()->layout()->addWidget(allFullScreenWidgets[0]);
    }
    else */if (allFullScreenWidgets.size() != 0)
    {   //  Need a tab control, with one page per FullScreenWidget
        _fullScreenTabWidget = new QTabWidget(this->centralWidget());
        this->centralWidget()->layout()->addWidget(_fullScreenTabWidget);
        _fullScreenTabWidget->setVisible(true);
        for (core::FullScreenWidget * fullScreenWidget : allFullScreenWidgets)
        {
            _fullScreenTabWidget->addTab(fullScreenWidget,
                                         fullScreenWidget->component()->type()->smallIcon(),
                                         fullScreenWidget->displayName());
        }
    }

    //  Set up custom event handlers

    //  Done
    _loadPosition();
}

VirtualApplianceWindow::~VirtualApplianceWindow()
{
    delete _fullScreenTabWidget;
    delete _ui;
}

//////////
//  QWidget
void VirtualApplianceWindow::moveEvent(QMoveEvent * /*event*/)
{
    _savePosition();
}

void VirtualApplianceWindow::resizeEvent(QResizeEvent * /*event*/)
{
    _savePosition();
}

void VirtualApplianceWindow::closeEvent(QCloseEvent * event)
{
    _virtualAppliance->stop();
    event->ignore();
}

//////////
//  Implementation helpers
void VirtualApplianceWindow::_savePosition()
{
    QSettings settings("Aka", "EmuOne");
    int x = settings.value("VirtualApplianceWindow-" + _virtualAppliance->name() + "/X", this->x()).toInt();
    int y = settings.value("VirtualApplianceWindow-" + _virtualAppliance->name() + "/Y", this->y()).toInt();
    int width = settings.value("VirtualApplianceWindow-" + _virtualAppliance->name() + "/Width", this->width()).toInt();
    int height = settings.value("VirtualApplianceWindow-" + _virtualAppliance->name() + "/Height", this->height()).toInt();
    this->setGeometry(x, y, width, height);
}

void VirtualApplianceWindow::_loadPosition()
{
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
        msgBox.setText(ex.message());
        msgBox.exec();
    }
}

//  End of emuone/VirtualApplianceWindow.cpp
