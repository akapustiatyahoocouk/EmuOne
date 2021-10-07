//
//  emuone/NewVmDialog.hpp
//
//  "New VM" dialog
//
//////////
#pragma once
#include "emuone/API.hpp"
#include <QDialog>

namespace Ui { class NewVmDialog; }

//////////
//  "New VM" dialog
class NewVmDialog : public QDialog
{
    Q_OBJECT

    //////////
    //  Construction/destruction
public:
    explicit NewVmDialog(QWidget *parent = nullptr);
    ~NewVmDialog();

    //////////
    //  Operations
public:
    core::VirtualApplianceType *getVirtualApplianceType() const { return _virtualApplianceType; }
    core::Architecture *        getVirtualApplianceArchitecture() const { return _virtualApplianceArchitecture; }
    core::VirtualApplianceTemplate *    getVirtualApplianceTemplate() const { return _virtualApplianceTemplate; }
    QString                     getVirtualApplianceName() const { return _virtualApplianceName; }
    QString                     getVirtualApplianceLocation() const { return _virtualApplianceLocation; }

    //////////
    //  Implementation
private:
    Ui::NewVmDialog *ui;
    core::VirtualApplianceType *_virtualApplianceType = nullptr;
    core::Architecture *        _virtualApplianceArchitecture = nullptr;
    core::VirtualApplianceTemplate *    _virtualApplianceTemplate = nullptr;
    QString                     _virtualApplianceName;
    QString                     _virtualApplianceLocation;

    //  Helpers
    void                _refresh();
    void                _refreshTemplatesList();
    core::Architecture *_getSelectedArchitecture();
    core::VirtualApplianceTemplate *    _getSelectedTemplate();

    //////////
    //  Event handflers
private slots:
    void                _architectureComboBoxCurrentIndexChanged(int index);
    void                _templateComboBoxCurrentIndexChanged(int index);
    void                _nameLineEditTextChanged(const QString &);
    void                _locationLineEditTextChanged(const QString &);
    void                _browse();
    void                _accept();
    void                _reject();
};

//  End of emuone/NewVmDialog.hpp
