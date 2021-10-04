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
    _loadVirtualAppliances();

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

void MainWindow::_loadVirtualAppliances()
{
    QSettings settings("Aka", "EmuOne");
    int count = settings.value("VirtualAppliances/Count", 0).toInt();
    for (int i = 0; i < count; i++)
    {
        QString location = settings.value("VirtualAppliances/Location" + QString::number(i), "").toString();
        if (_findVirtualApplianceByLocation(location) == nullptr)
        {
            try
            {
                _virtualAppliances.append(VirtualAppliance::load(location));
            }
            catch (VirtualApplianceException & ex)
            {   //  OOPS! Suppress, though
            }
        }
    }
}

void MainWindow::_saveVirtualAppliances()
{
    QSettings settings("Aka", "EmuOne");
    settings.setValue("VirtualAppliances/Count", _virtualAppliances.size());
    for (int i = 0; i < _virtualAppliances.size(); i++)
    {
        settings.setValue("VirtualAppliances/Location" + QString::number(i), _virtualAppliances[i]->getLocation());
    }
}

VirtualAppliance * MainWindow::_getSelectedVirtualAppliance() const
{
    if (_virtualAppliances.size() != ui->vmListWidget->count())
    {
        const_cast<MainWindow*>(this)->_refreshVirtualAppliancesList();
    }
    int rowIndex = ui->vmListWidget->currentRow();
    return (rowIndex >= 0) ? _virtualAppliances[rowIndex] : nullptr;
}

void MainWindow::_setSelectedVirtualAppliance(VirtualAppliance * virtualAppliance)
{
    _refreshVirtualAppliancesList();
    for (int i = 0; i < _virtualAppliances.size(); i++)
    {
        if (_virtualAppliances[i] == virtualAppliance)
        {
            ui->vmListWidget->setCurrentRow(i);
            break;
        }
    }
}

void MainWindow::_refreshVirtualAppliancesList()
{
    //  Make sure the VM list has a proper number of items...
    while (_virtualAppliances.size() < ui->vmListWidget->count())
    {   //  Too many items in the list widget
        delete ui->vmListWidget->takeItem(0);
    }
    while (_virtualAppliances.size() > ui->vmListWidget->count())
    {   //  Too few items in the list widget
        ui->vmListWidget->addItem(".");
    }
    //  ...with correct properties
    for (int i = 0; i < _virtualAppliances.size(); i++)
    {
        QListWidgetItem * item = ui->vmListWidget->item(i);
        QString text = _virtualAppliances[i]->getName() +
                       " (" + _virtualAppliances[i]->getType()->getDisplayName() + ")";
        //  TODO state
        item->setText(text);
        item->setIcon(_virtualAppliances[i]->getArchitecture()->getSmallIcon());
    }
}

void MainWindow::_refresh()
{
    VirtualAppliance * virtualAppliance = _getSelectedVirtualAppliance();

    this->ui->actionCloseVm->setEnabled(virtualAppliance != nullptr);
}

VirtualAppliance * MainWindow::_findVirtualApplianceByLocation(const QString & location)
{
    for (VirtualAppliance * virtualAppliance : _virtualAppliances)
    {
        if (virtualAppliance->getLocation() == location)
        {
            return virtualAppliance;
        }
    }
    return nullptr;
}

//////////
//  Event handlers
void MainWindow::_onNewVmTriggered()
{
    NewVmDialog newVmDialog(this);
    if (newVmDialog.exec() == QDialog::DialogCode::Accepted)
    {   //  Create a new VA
        VirtualAppliance * virtualAppliance =
            newVmDialog.getVirtualApplianceType()->createVirtualAppliance(
                newVmDialog.getVirtualApplianceName(),
                newVmDialog.getVirtualApplianceLocation(),
                newVmDialog.getVirtualApplianceArchitecture(),
                newVmDialog.getVirtualApplianceTemplate());
        try
        {
            virtualAppliance->save();
        }
        catch (VirtualApplianceException & ex)
        {   //  OOPS! Report & abort
            QMessageBox msgBox;
            msgBox.setText(ex.getMessage());
            msgBox.exec();
            return;
        }
        //  If there already exists a VA with the same "location"< stop & drop it
        VirtualAppliance * existingVirtualAppliance = _findVirtualApplianceByLocation(newVmDialog.getVirtualApplianceLocation());
        if (existingVirtualAppliance != nullptr)
        {
            _virtualAppliances.removeOne(existingVirtualAppliance);
            delete existingVirtualAppliance;
        }
        //  Add & sort
        _virtualAppliances.append(virtualAppliance);
        std::sort(_virtualAppliances.begin(), _virtualAppliances.end(),
                  [](const VirtualAppliance * a, const VirtualAppliance * b) -> bool { return a->getName() < b->getName(); });
        _saveVirtualAppliances();
        _refreshVirtualAppliancesList();
        _refresh();
        //  Select newly creates VA as "current"
        _setSelectedVirtualAppliance(virtualAppliance);
    }
}

void MainWindow::_onOpenVmTriggered()
{
    QString location = QFileDialog::getOpenFileName(this, "VM location", "", "EmuOne VM file (*." + VirtualAppliance::PreferredExtension + ")");
    if (location.isEmpty())
    {   //  User has cancelled the dialog
        return;
    }
    //  Is there already an open VA with the same "location" ?
    VirtualAppliance * virtualAppliance = _findVirtualApplianceByLocation(location);
    if (virtualAppliance != nullptr)
    {   //  Yes - just select it as "current"
        this->_setSelectedVirtualAppliance(virtualAppliance);
        return;
    }
    //  Open...
    try
    {
        virtualAppliance = VirtualAppliance::load(location);
    }
    catch (VirtualApplianceException & ex)
    {   //  OOPS! Report & abort
        QMessageBox msgBox;
        msgBox.setText(ex.getMessage());
        msgBox.exec();
        return;
    }
    //  Add & sort
    _virtualAppliances.append(virtualAppliance);
    std::sort(_virtualAppliances.begin(), _virtualAppliances.end(),
              [](const VirtualAppliance * a, const VirtualAppliance * b) -> bool { return a->getName() < b->getName(); });
    _saveVirtualAppliances();
    _refreshVirtualAppliancesList();
    _refresh();
    //  Select newly creates VA as "current"
    _setSelectedVirtualAppliance(virtualAppliance);
}

void MainWindow::_onCloseVmTriggered()
{
}

void MainWindow::_onExitTriggered()
{
    QApplication::quit();
}

//  End of emuone/MainWindow.cpp
