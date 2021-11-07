//
//  emuone-ibm360/Ibm2741Editor.hpp
//
//  "IBM 2741 editor" contyrol
//
//////////
#pragma once
#include "emuone-ibm360/API.hpp"
#include <QWidget>

namespace ibm360
{
    namespace Ui { class Ibm2741Editor; }

    //////////
    //  The IBM 2741 editor
    class EMUONE_IBM360_EXPORT Ibm2741Editor : public ComponentEditor
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Ibm2741Editor)

        //////////
        //  Construction/destruction
    public:
        Ibm2741Editor(ibm360::Ibm2741 * ibm2741, QWidget * parent = nullptr);
        ~Ibm2741Editor();

        //////////
        //  ComponentEditor
    public:
        virtual void            refresh() override;

        //////////
        //  Implementation
    private:
        Ui::Ibm2741Editor *     _ui;
        ibm360::Ibm2741 *const  _ibm2741;
        bool                    _refreshUnderway = false;

        //////////
        //  Event handlers
    private slots:
        void                    _addressLineEditTextChanged(const QString &);
    };
}

//  End of emuone-ibm360/Ibm2741Editor.hpp
