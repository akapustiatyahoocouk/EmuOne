//
//  emuone/ConfigureVmDialog.hpp
//
//  "Configure VM" dialog
//
//////////
#pragma once
#include "emuone/API.hpp"
#include <QDialog>

namespace Ui { class ConfigureVmDialog; }

//////////
//  The "Configure VM" dialog
class ConfigureVmDialog : public QDialog
{
    Q_OBJECT

    //////////
    //  Construction/destruction
public:
    ConfigureVmDialog(VirtualAppliance * virtualAppliance, QWidget * parent = nullptr);
    ~ConfigureVmDialog();

    //////////
    //  Implementation
private:
    Ui::ConfigureVmDialog * ui;
    VirtualAppliance *      _virtualAppliance;

    //  Helpers
    void                    _refreshComponentsTree();
    void                    _refresh();

    //////////
    //  Event handlers
private slots:
    void                    _addComponentPushButtonClicked();
    void                    _removeComponentPushButtonClicked();
    void                    _accept();
    void                    _reject();
};

//  End of emuone/ConfigureVmDialog.hpp
