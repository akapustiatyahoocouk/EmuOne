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
    VirtualApplianceWindow(VirtualAppliance * virtualAppliance, QWidget * parent = nullptr);
    virtual ~VirtualApplianceWindow();

    //////////
    //  QWidget
protected:
    virtual void        closeEvent(QCloseEvent * event) override;

    //////////
    //  Implementation
private:
    Ui::VirtualApplianceWindow *ui;
    VirtualAppliance *const _virtualAppliance;
};

//  End of emuone/VirtualApplianceWindow.hpp

