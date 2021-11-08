//
//  emuone-core/ResidentRamUnit64Editor.hpp
//
//  VM core exceptions
//
//////////
#pragma once
#include "emuone-core/API.hpp"
#include <QWidget>

namespace core
{
    namespace Ui { class ResidentRamUnit64Editor; }

    //////////
    //  The editor of ResidentRamUnit64 properties
    class EMUONE_CORE_EXPORT ResidentRamUnit64Editor : public ComponentEditor
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ResidentRamUnit64Editor)

        //////////
        //  Construction/destruction
    public:
        explicit ResidentRamUnit64Editor(ResidentRamUnit64 * ramInit, QWidget * parent = nullptr);
        virtual ~ResidentRamUnit64Editor();

        //////////
        //  ComponentEditor
    public:
        virtual void                    refresh() override;

        //////////
        //  Implementation
    private:
        Ui::ResidentRamUnit64Editor *   _ui;
        ResidentRamUnit64 *             _ramInit;

        bool                            _refreshUnderway = false;
    };
}

//  End of emuone-core/ResidentRamUnit64Editor.hpp

