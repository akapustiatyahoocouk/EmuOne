//
//  emuone-core/ResidentRomUnit64Editor.hpp
//
//  "Resident ROM unit editor" control
//
//////////
#pragma once
#include "emuone-ibm360/API.hpp"
#include <QWidget>

namespace core {

    namespace Ui { class ResidentRomUnit64Editor; }

    //////////
    //  "Resident ROM unit editor" control
    class ResidentRomUnit64Editor : public ComponentEditor
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ResidentRomUnit64Editor)

        //////////
        //  Construction/destruction
    public:
        explicit ResidentRomUnit64Editor(ResidentRomUnit64 * romUnit, QWidget * parent = nullptr);
        ~ResidentRomUnit64Editor();

        //////////
        //  ComponentEditor
    public:
        virtual void        refresh() override;

        //////////
        //  Implementation
    private:
        Ui::ResidentRomUnit64Editor *   _ui;
        ResidentRomUnit64 *             _romUnit;
        bool                            _refreshUnderway = false;

        //////////
        //  Event handlers
    private slots:
        void                            _addressLineEditTextChanged(const QString &);
        void                            _sizeLineEditTextChanged(const QString &);
        void                            _sizeUnitComboBoxCurrentIndexChanged(int);
        void                            _browsePushButtonClicked();
    };
}

//  End of emuone-core/ResidentRomUnit64Editor.hpp

