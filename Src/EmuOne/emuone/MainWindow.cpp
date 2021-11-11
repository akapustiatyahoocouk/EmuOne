//
//  emuone/MainWindow.cpp
//
//  Main UI frame
//
//////////
#include "emuone/API.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ui_MainWindow.h"
#pragma GCC diagnostic pop

//////////
//  Construction/destruction
MainWindow::MainWindow()
    :   QMainWindow(nullptr),
        _ui(new Ui::MainWindow),
        _refreshTimer(this)
{
    _ui->setupUi(this);

    _loadPosition();
    _loadVirtualAppliances();

    //  Set up custom event handlers
    connect(&_refreshTimer, &QTimer::timeout, this, &MainWindow::_refreshTimerTimeout);

    //  Done
    _trackPositionChanges = true;
    _refresh();
    _refreshTimer.start(250);
}

MainWindow::~MainWindow()
{
    _refreshTimer.stop();
    delete _ui;
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

void MainWindow::closeEvent(QCloseEvent * /*event*/)
{
    _onExitTriggered();
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
                _virtualAppliances.append(core::VirtualAppliance::load(location));
            }
            catch (core::VirtualApplianceException & ex)
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
        settings.setValue("VirtualAppliances/Location" + QString::number(i), _virtualAppliances[i]->location());
    }
}

core::VirtualAppliance * MainWindow::_getSelectedVirtualAppliance() const
{
    if (_virtualAppliances.size() != _ui->vmListWidget->count())
    {
        const_cast<MainWindow*>(this)->_refreshVirtualAppliancesList();
    }
    int rowIndex = _ui->vmListWidget->currentRow();
    return (rowIndex >= 0) ? _virtualAppliances[rowIndex] : nullptr;
}

void MainWindow::_setSelectedVirtualAppliance(core::VirtualAppliance * virtualAppliance)
{
    _refreshVirtualAppliancesList();
    for (int i = 0; i < _virtualAppliances.size(); i++)
    {
        if (_virtualAppliances[i] == virtualAppliance)
        {
            _ui->vmListWidget->setCurrentRow(i);
            break;
        }
    }
}

void MainWindow::_refreshVirtualAppliancesList()
{
    //  Make sure the VM list has a proper number of items...
    while (_virtualAppliances.size() < _ui->vmListWidget->count())
    {   //  Too many items in the list widget
        _ui->vmListWidget->takeItem(0);
    }
    while (_virtualAppliances.size() > _ui->vmListWidget->count())
    {   //  Too few items in the list widget
        _ui->vmListWidget->addItem(".");
    }
    //  ...with correct properties
    for (int i = 0; i < _virtualAppliances.size(); i++)
    {
        QListWidgetItem * item = _ui->vmListWidget->item(i);
        QString text = _virtualAppliances[i]->name() +
                       " (" + _virtualAppliances[i]->architecture()->displayName() +
                              " " + _virtualAppliances[i]->type()->displayName() + ")";
        switch (_virtualAppliances[i]->state())
        {
            case core::VirtualAppliance::State::Stopped:
                break;
            case core::VirtualAppliance::State::Running:
                text += " - running";
                break;
            case core::VirtualAppliance::State::Suspended:
                text += " - suspended";
                break;
            default:
                text += " - ?";
                break;
        }
        item->setText(text);
        item->setIcon(_virtualAppliances[i]->architecture()->smallIcon());
    }
}

void MainWindow::_refresh()
{
    core::VirtualAppliance * virtualAppliance = _getSelectedVirtualAppliance();

    _ui->actionCloseVm->setEnabled(virtualAppliance != nullptr);

    _ui->actionStartVm->setEnabled(virtualAppliance != nullptr && virtualAppliance->state() == core::VirtualAppliance::State::Stopped);
    _ui->actionStopVm->setEnabled(virtualAppliance != nullptr && virtualAppliance->state() != core::VirtualAppliance::State::Stopped);
    _ui->actionSuspendVm->setEnabled(virtualAppliance != nullptr && virtualAppliance->state() == core::VirtualAppliance::State::Running);
    _ui->actionResumeVm->setEnabled(virtualAppliance != nullptr && virtualAppliance->state() == core::VirtualAppliance::State::Suspended);
    _ui->actionConfigureVm->setEnabled(virtualAppliance != nullptr && virtualAppliance->state() == core::VirtualAppliance::State::Stopped);

    _ui->startButton->setEnabled(_ui->actionStartVm->isEnabled());
    _ui->stopButton->setEnabled(_ui->actionStopVm->isEnabled());
    _ui->suspendButton->setEnabled(_ui->actionSuspendVm->isEnabled());
    _ui->resumeButton->setEnabled(_ui->actionResumeVm->isEnabled());
    _ui->configureButton->setEnabled(_ui->actionConfigureVm->isEnabled());
}

core::VirtualAppliance * MainWindow::_findVirtualApplianceByLocation(const QString & location)
{
    for (core::VirtualAppliance * virtualAppliance : _virtualAppliances)
    {
        if (virtualAppliance->location() == location)
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
        core::VirtualAppliance * virtualAppliance =
            newVmDialog.virtualApplianceType()->createVirtualAppliance(
                newVmDialog.virtualApplianceName(),
                newVmDialog.virtualApplianceLocation(),
                newVmDialog.virtualApplianceArchitecture(),
                newVmDialog.virtualApplianceTemplate());
        try
        {
            newVmDialog.virtualApplianceTemplate()->populateVirtualAppliance(virtualAppliance);
            virtualAppliance->save();
        }
        catch (core::VirtualApplianceException & ex)
        {   //  OOPS! Report & abort
            QMessageBox msgBox;
            msgBox.setText(ex.message());
            msgBox.exec();
            return;
        }
        //  If there already exists a VA with the same "location"< stop & drop it
        core::VirtualAppliance * existingVirtualAppliance = _findVirtualApplianceByLocation(newVmDialog.virtualApplianceLocation());
        if (existingVirtualAppliance != nullptr)
        {
            _virtualAppliances.removeOne(existingVirtualAppliance);
            delete existingVirtualAppliance;
        }
        //  Add & sort
        _virtualAppliances.append(virtualAppliance);
        std::sort(_virtualAppliances.begin(), _virtualAppliances.end(),
                  [](auto a, auto b) -> bool { return a->name() < b->name(); });
        _saveVirtualAppliances();
        _refreshVirtualAppliancesList();
        _refresh();
        //  Select newly creates VA as "current"
        _setSelectedVirtualAppliance(virtualAppliance);
        //  ...and start editing its configuration
        _onConfigureVmTriggered();
    }
}

void MainWindow::_onOpenVmTriggered()
{
    QString location = QFileDialog::getOpenFileName(this, "VM location", "", "EmuOne VM file (*." + core::VirtualAppliance::PreferredExtension + ")");
    if (location.isEmpty())
    {   //  User has cancelled the dialog
        return;
    }
    //  Is there already an open VA with the same "location" ?
    core::VirtualAppliance * virtualAppliance = _findVirtualApplianceByLocation(location);
    if (virtualAppliance != nullptr)
    {   //  Yes - just select it as "current"
        this->_setSelectedVirtualAppliance(virtualAppliance);
        return;
    }
    //  Open...
    try
    {
        virtualAppliance = core::VirtualAppliance::load(location);
    }
    catch (core::VirtualApplianceException & ex)
    {   //  OOPS! Report & abort
        QMessageBox msgBox;
        msgBox.setText(ex.message());
        msgBox.exec();
        return;
    }
    //  Add & sort
    _virtualAppliances.append(virtualAppliance);
    std::sort(_virtualAppliances.begin(), _virtualAppliances.end(),
              [](auto a, auto b) -> bool { return a->name() < b->name(); });
    _saveVirtualAppliances();
    _refreshVirtualAppliancesList();
    _refresh();
    //  Select newly creates VA as "current"
    _setSelectedVirtualAppliance(virtualAppliance);
}

void MainWindow::_onCloseVmTriggered()
{
    core::VirtualAppliance * virtualAppliance = _getSelectedVirtualAppliance();
    if (virtualAppliance != nullptr)
    {   //  Confirm...
        QString message =
            "Are you sure you want to close the " + virtualAppliance->type()->displayName() +
            " " + virtualAppliance->name() + "?";
        QMessageBox::StandardButton reply =
            QMessageBox::question(this, "Close VM", message, QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {   //  ...and close
            virtualAppliance->stop();   //  ...just in case
            _virtualAppliances.removeOne(virtualAppliance);
            _saveVirtualAppliances();
            _refreshVirtualAppliancesList();
            _refresh();
            delete virtualAppliance;
        }
    }
}

void MainWindow::_onExitTriggered()
{   //  Stop all running VAs...
    for (core::VirtualAppliance * virtualAppliance : _virtualAppliances)
    {
        if (virtualAppliance->state() == core::VirtualAppliance::State::Running)
        {
            virtualAppliance->stop();
        }
    }
    //  ...and exit
    QApplication::quit();
}

void MainWindow::_onStartVmTriggered()
{
    try
    {
        core::VirtualAppliance * virtualAppliance = _getSelectedVirtualAppliance();
        if (virtualAppliance != nullptr)
        {
            virtualAppliance->start();
            _refreshVirtualAppliancesList();
            _refresh();
            //  Create UI for the VA
            VirtualApplianceWindow * virtualApplianceWindow = new VirtualApplianceWindow(virtualAppliance);
            virtualApplianceWindow->setVisible(true);
            _virtualApplianceWindows.insert(virtualAppliance, virtualApplianceWindow);
        }
    }
    catch (core::VirtualApplianceException & ex)
    {
        QMessageBox msgBox;
        msgBox.setText(ex.message());
        msgBox.exec();
    }
}

void MainWindow::_onStopVmTriggered()
{
    core::VirtualAppliance * virtualAppliance = _getSelectedVirtualAppliance();
    if (virtualAppliance != nullptr)
    {
        //  Is there a VA UI window we need to close ?
        if (_virtualApplianceWindows.contains(virtualAppliance))
        {   //  Yes!
            VirtualApplianceWindow * virtualApplianceWindow = _virtualApplianceWindows[virtualAppliance];
            _virtualApplianceWindows.remove(virtualAppliance);
            delete virtualApplianceWindow;
        }
        //  Stop the VA
        virtualAppliance->stop();
        _refreshVirtualAppliancesList();
        _refresh();
    }
}

void MainWindow::_onSuspendVmTriggered()
{
    try
    {
        core::VirtualAppliance * virtualAppliance = _getSelectedVirtualAppliance();
        if (virtualAppliance != nullptr)
        {
            virtualAppliance->suspend();
            _refreshVirtualAppliancesList();
            _refresh();
        }
    }
    catch (core::VirtualApplianceException & ex)
    {
        QMessageBox msgBox;
        msgBox.setText(ex.message());
        msgBox.exec();
    }
}

void MainWindow::_onResumeVmTriggered()
{
    try
    {
        core::VirtualAppliance * virtualAppliance = _getSelectedVirtualAppliance();
        if (virtualAppliance != nullptr)
        {
            virtualAppliance->resume();
            _refreshVirtualAppliancesList();
            _refresh();
        }
    }
    catch (core::VirtualApplianceException & ex)
    {
        QMessageBox msgBox;
        msgBox.setText(ex.message());
        msgBox.exec();
    }
}

void MainWindow::_onConfigureVmTriggered()
{
    core::VirtualAppliance * virtualAppliance = _getSelectedVirtualAppliance();
    if (virtualAppliance != nullptr)
    {
        ConfigureVmDialog dlg(virtualAppliance, this);
        if (dlg.exec() == QDialog::DialogCode::Accepted)
        {   //  VA may have been renamed!
            _refreshVirtualAppliancesList();
            _refresh();
            _setSelectedVirtualAppliance(virtualAppliance);
        }
    }
}

void MainWindow::_onVmListCurrentRowChanged(int)
{
    _refresh();
}

void MainWindow::_refreshTimerTimeout()
{
    //  Has any VA that was "running" stopped ?
    for (core::VirtualAppliance * virtualAppliance : _virtualAppliances)
    {
        if (_virtualApplianceWindows.contains(virtualAppliance) &&
            virtualAppliance->state() != core::VirtualAppliance::State::Running)
        {   //  Must kill this VA window
            VirtualApplianceWindow * virtualApplianceWindow = _virtualApplianceWindows[virtualAppliance];
            _virtualApplianceWindows.remove(virtualAppliance);
            delete virtualApplianceWindow;
        }
    }
    //  Refresh UI
    _refreshVirtualAppliancesList();
    _refresh();
}

//  End of emuone/MainWindow.cpp
