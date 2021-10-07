//
//  emuone/MainWindow.hpp
//
//  Main UI frame
//
//////////
#pragma once
#include "emuone/API.hpp"
#include <QMainWindow>

class VirtualApplianceWindow;

namespace Ui { class MainWindow; }

//////////
//  The main UI frame
class MainWindow final : public QMainWindow
{
    Q_OBJECT

    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MainWindow)

    //////////
    //  Construction/destruction
public:
    MainWindow(QWidget * parent = nullptr);
    ~MainWindow();

    //////////
    //  QWidget
protected:
    void                    moveEvent(QMoveEvent * event) override;
    void                    resizeEvent(QResizeEvent * event) override;

    //////////
    //  Implementation
private:
    bool                    _trackPositionChanges = false;
    core::VirtualApplianceList  _virtualAppliances; //  ...in view order

    QMap<core::VirtualAppliance*,VirtualApplianceWindow*> _virtualApplianceWindows;   //  all that currently exist

    //  Helpers
    void                    _loadPosition();
    void                    _savePosition();
    void                    _loadVirtualAppliances();
    void                    _saveVirtualAppliances();

    core::VirtualAppliance *_getSelectedVirtualAppliance() const;
    void                    _setSelectedVirtualAppliance(core::VirtualAppliance * virtualAppliance);
    void                    _refreshVirtualAppliancesList();
    void                    _refresh();

    core::VirtualAppliance *_findVirtualApplianceByLocation(const QString & location);

    //////////
    //  Controls
private:
    Ui::MainWindow *        ui;
    QTimer                  _refreshTimer;

    //////////
    //  Event handlers
private slots:
    void                    _onNewVmTriggered();
    void                    _onOpenVmTriggered();
    void                    _onCloseVmTriggered();
    void                    _onExitTriggered();
    void                    _onStartVmTriggered();
    void                    _onStopVmTriggered();
    void                    _onSuspendVmTriggered();
    void                    _onResumeVmTriggered();
    void                    _onConfigureVmTriggered();
    void                    _onVmListCurrentRowChanged(int);
    void                    _refreshTimerTimeout();
};

//  End of emuone/MainWindow.hpp
