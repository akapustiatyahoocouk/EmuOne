//
//  emuone-cereon/Cmos1Editor.hpp
//
//  Cmos1 unit editor
//
//////////
#pragma once
#include "emuone-cereon/API.hpp"
#include <QWidget>

namespace cereon
{

    namespace Ui { class Cmos1Editor; }

    //////////
    //  Cmos1 unit editor
    class EMUONE_CEREON_EXPORT Cmos1Editor : public core::ComponentEditor
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Cmos1Editor)

        //////////
        //  Construction/destruction
    public:
        explicit Cmos1Editor(Cmos1 * cmos1, QWidget * parent = nullptr);
        ~Cmos1Editor();

        //////////
        //  core::ComponentEditor
    public:
        virtual void            refresh() override;

        //////////
        //  Implementation
    private:
        Ui::Cmos1Editor *       _ui;
        Cmos1 *const            _cmos1;
        bool                    _refreshUnderway = false;

        //////////
        //  Event listeners
    private slots:
        void                    _statePortLineEditTextChanged(const QString &);
        void                    _addressPortLineEditTextChanged(const QString &);
        void                    _dataPortLineEditTextChanged(const QString &);
        void                    _interruptMaskPortLineEditTextChanged(const QString &);
    };
}

//  Ed of emuone-cereon/Cmos1Editor.hpp
