//
//  emuone-core/ResidentRamUnit64Editor.hpp
//
//  "Resident RAM unit editor" control
//
//////////
#pragma once
#include "emuone-ibm360/API.hpp"
#include <QWidget>

namespace core {

    namespace Ui { class ResidentRamUnit64Editor; }

    //////////
    //  "Resident RAM unit editor" control
    class EMUONE_IBM360_EXPORT ResidentRamUnit64Editor : public ComponentEditor
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ResidentRamUnit64Editor)

        //////////
        //  Construction/destruction
    public:
        explicit ResidentRamUnit64Editor(ResidentRamUnit64 * ramUnit, QWidget * parent = nullptr);
        virtual ~ResidentRamUnit64Editor();

        //////////
        //  ComponentEditor
    public:
        virtual void        refresh() override;

        //////////
        //  Implementation
    private:
        Ui::ResidentRamUnit64Editor *   _ui;
        ResidentRamUnit64 *             _ramUnit;
        bool                            _refreshUnderway = false;

        //////////
        //  Event handlers
    private slots:
        void                            _addressLineEditTextChanged(const QString &);
        void                            _sizeLineEditTextChanged(const QString &);
        void                            _sizeUnitComboBoxCurrentIndexChanged(int);
    };
}

//  End of emuone-core/ResidentRamUnit64Editor.hpp
