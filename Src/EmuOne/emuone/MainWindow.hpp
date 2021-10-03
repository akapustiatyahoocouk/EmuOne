//
//  emuone/MainWindow.hpp
//
//  Main UI frame
//
//////////
#pragma once
#include "emuone/API.hpp"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//////////
//  The main UI frame
class MainWindow : public QMainWindow
{
    Q_OBJECT

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
    VirtualApplianceList    _virtualAppliances; //  ...in view order

    //  Helpers
    void                    _loadPosition();
    void                    _savePosition();
    void                    _loadVirtualAppliances();
    void                    _saveVirtualAppliances();

    VirtualAppliance *      _getSelectedVirtualAppliance() const;
    void                    _setSelectedVirtualAppliance(VirtualAppliance * virtualAppliance);
    void                    _refreshVirtualAppliancesList();
    void                    _refresh();

    VirtualAppliance *      _findVirtualApplianceByLocation(const QString & location);

    //////////
    //  Controls
private:
    Ui::MainWindow *ui;

    //////////
    //  Event handlers
private slots:
    void                    _onNewVmTriggered();
    void                    _onOpenVmTriggered();
    void                    _onCloseVmTriggered();
    void                    _onExitTriggered();
};

//  End of emuone/MainWindow.hpp
