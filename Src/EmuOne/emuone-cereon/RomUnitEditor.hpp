//
//  emuone-cereon/RomUnitEditor.hpp
//
//  Cereon ROM unit editor
//
//////////
#pragma once
#include "emuone-cereon/API.hpp"
#include <QWidget>

namespace cereon {

    namespace Ui { class RomUnitEditor; }

    //////////
    //  Cereon ROM unit editor
    class RomUnitEditor : public core::ComponentEditor
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RomUnitEditor)

        //////////
        //  Construction/destruction
    public:
        explicit RomUnitEditor(RomUnit * romUnit, QWidget * parent = nullptr);
        virtual ~RomUnitEditor();

        //////////
        //  core::ComponentEditor
    public:
        virtual void            refresh() override;

        //////////
        //  Implementation
    private:
        Ui::RomUnitEditor *     _ui;
        RomUnit *               _romUnit;
        bool                    _refreshUnderway = false;

        //////////
        //  Event handlers
    private slots:
        void                    _addressLineEditTextChanged(const QString &);
        void                    _sizeLineEditTextChanged(const QString &);
        void                    _sizeUnitComboBoxCurrentIndexChanged(int);
        void                    _browsePushButtonClicked();
    };
}

//  End of emuone-cereon/RomUnitEditor.hpp
