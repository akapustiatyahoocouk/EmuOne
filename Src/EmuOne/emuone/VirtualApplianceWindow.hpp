//
//  emuone/VirtualApplianceWindow.hpp
//
//  VA UI frame
//
//////////
#pragma once
#include "emuone/API.hpp"
#include <QMainWindow>

namespace Ui { class VirtualApplianceWindow; }

//////////
//  A window representing a single running VA
class VirtualApplianceWindow final : public QMainWindow
{
    Q_OBJECT

    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(VirtualApplianceWindow)

    //////////
    //  Construction/destruction
public:
    VirtualApplianceWindow(core::VirtualAppliance * virtualAppliance, QWidget * parent = nullptr);
    virtual ~VirtualApplianceWindow();

    //////////
    //  QWidget
protected:
    virtual void        moveEvent(QMoveEvent * event) override;
    virtual void        resizeEvent(QResizeEvent * event) override;
    virtual void        closeEvent(QCloseEvent * event) override;

    //////////
    //  Implementation
private:
    Ui::VirtualApplianceWindow *    _ui;
    core::VirtualAppliance *const   _virtualAppliance;

    QMap<core::Component*, core::ComponentUi*>  _componentUiMap = {};
    QMap<core::Adaptor*, core::AdaptorUi*>      _adaptorUiMap = {};

    //  Helpers
    void                _savePosition();
    void                _loadPosition();

    //////////
    //  Controls
private:
    QTabWidget *        _fullScreenTabWidget = nullptr;

    //////////
    //  Event handlers
private slots:
    void                _actionStopTriggered();
    void                _actionSuspendTriggered();
};

//  End of emuone/VirtualApplianceWindow.hpp

