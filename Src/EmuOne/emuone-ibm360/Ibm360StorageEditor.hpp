//
//  emuone-ibm360/NewVmDialog.hpp
//
//  "IBM/360 storage editor" contyrol
//
//////////
#pragma once
#include "emuone-ibm360/API.hpp"
#include <QWidget>

namespace Ui { class Ibm360StorageEditor; }

//////////
//  The IBM/360 storage editor
class EMUONE_IBM360_EXPORT Ibm360StorageEditor : public ComponentEditor
{
    Q_OBJECT

    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Ibm360StorageEditor)

    //////////
    //  Construction/destruction
public:
    Ibm360StorageEditor(Ibm360Storage * ibm360Storage, QWidget * parent);
    virtual ~Ibm360StorageEditor();

    //////////
    //  ComponentEditor
public:
    virtual void    refresh() override;

    //////////
    //  Implementation
private:
    Ui::Ibm360StorageEditor *ui;
    Ibm360Storage *     _ibm360Storage;
    bool                _refreshUnderway = false;

    //////////
    //  Event handlers
private slots:
    void                _sizeLineEditTextChanged(const QString &);
    void                _sizeUnitComboBoxCurrentIndexChanged(int);
};

//  End of emuone-ibm360/NewVmDialog.hpp
