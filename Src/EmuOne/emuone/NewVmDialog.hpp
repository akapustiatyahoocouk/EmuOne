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
class NewVmDialog final : public QDialog
{
    Q_OBJECT

    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(NewVmDialog)

    //////////
    //  Construction/destruction
public:
    explicit NewVmDialog(QWidget * parent);
    ~NewVmDialog();

    //////////
    //  Operations
public:
    core::VirtualApplianceType *virtualApplianceType() const { return _virtualApplianceType; }
    core::Architecture *        virtualApplianceArchitecture() const { return _virtualApplianceArchitecture; }
    core::VirtualApplianceTemplate *    virtualApplianceTemplate() const { return _virtualApplianceTemplate; }
    QString                     virtualApplianceName() const { return _virtualApplianceName; }
    QString                     virtualApplianceLocation() const { return _virtualApplianceLocation; }

    //////////
    //  Implementation
private:
    Ui::NewVmDialog *           _ui;
    core::VirtualApplianceType *_virtualApplianceType = nullptr;
    core::Architecture *        _virtualApplianceArchitecture = nullptr;
    core::VirtualApplianceTemplate *    _virtualApplianceTemplate = nullptr;
    QString                     _virtualApplianceName = {};
    QString                     _virtualApplianceLocation = {};

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
