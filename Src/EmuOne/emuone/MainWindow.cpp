//
//  emuone/MainWindow.cpp
//
//  Main UI frame
//
//////////
#include "emuone/API.hpp"

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _loadPosition();

    //  Done
    _trackPositionChanges = true;
    _refresh();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//////////
//  QWidget
void MainWindow::moveEvent(QMoveEvent * /*event*/)
{
    _savePosition();
}

void MainWindow::resizeEvent(QResizeEvent * /*event*/)
{
    _savePosition();
}

//////////
//  Implementation helpers
void MainWindow::_loadPosition()
{
    QSettings settings("Aka", "EmuOne");
    int x = settings.value("MainWindow/X", this->x()).toInt();
    int y = settings.value("MainWindow/Y", this->y()).toInt();
    int width = settings.value("MainWindow/Width", this->width()).toInt();
    int height = settings.value("MainWindow/Height", this->height()).toInt();
    this->setGeometry(x, y, width, height);
}

void MainWindow::_savePosition()
{
    if (_trackPositionChanges)
    {
        QSettings settings("Aka", "EmuOne");
        if (!this->isMaximized() && !this->isMinimized())
        {
            QRect rc = this->frameGeometry();
            settings.setValue("MainWindow/X", rc.left());
            settings.setValue("MainWindow/Y", rc.top());
            settings.setValue("MainWindow/Width", rc.width());
            settings.setValue("MainWindow/Height", rc.height());
        }
    }
}

VirtualAppliance * MainWindow::_getSelectedVirtualAppliance() const
{
    return nullptr;
}

void MainWindow::_refresh()
{
    VirtualAppliance * virtualAppliance = _getSelectedVirtualAppliance();

    this->ui->actionCloseVm->setEnabled(virtualAppliance != nullptr);
}

//////////
//  Event handlers
void MainWindow::_onNewVmTriggered()
{
    NewVmDialog newVmDialog(this);
    newVmDialog.exec();
}

void MainWindow::_onOpenVmTriggered()
{
}

void MainWindow::_onCloseVmTriggered()
{
}

void MainWindow::_onExitTriggered()
{
    QApplication::quit();
}

//  End of emuone/MainWindow.cpp
