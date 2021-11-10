//
//  emuone-cereon/RamUnitEditor.hpp
//
//  Cereon RAM unit editor
//
//////////
#pragma once
#include "emuone-cereon/API.hpp"
#include <QWidget>

namespace cereon
{
    namespace Ui { class RamUnitEditor; }

    //////////
    //  Cereon RAM unit editor
    class EMUONE_CEREON_EXPORT RamUnitEditor : public core::ComponentEditor
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RamUnitEditor)

        //////////
        //  Construction/destruction
    public:
        explicit RamUnitEditor(RamUnit * ramUnit, QWidget * parent = nullptr);
        virtual ~RamUnitEditor();

        //////////
        //  core::ComponentEditor
    public:
        virtual void            refresh() override;

        //////////
        //  Implementation
    private:
        Ui::RamUnitEditor *     _ui;
        RamUnit *               _ramUnit;
        bool                    _refreshUnderway = false;

        //////////
        //  Event handlers
    private slots:
        void                    _addressLineEditTextChanged(const QString &);
        void                    _sizeLineEditTextChanged(const QString &);
        void                    _sizeUnitComboBoxCurrentIndexChanged(int);
    };
}

//  End of emuone-cereon/RamUnitEditor.hpp
