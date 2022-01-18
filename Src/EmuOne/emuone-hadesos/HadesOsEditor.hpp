//
//  emuone-hadesos/HadesOsEditor.hpp
//
//  Hades OS unit editor
//
//////////
#pragma once
#include "emuone-hadesos/API.hpp"
#include <QWidget>

namespace hadesos
{
    namespace Ui { class HadesOsEditor; }

    //////////
    //  The Hades OS unit editor
    class EMUONE_HADESOS_EXPORT HadesOsEditor : public core::ComponentEditor
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(HadesOsEditor)

        //////////
        //  Construction/destruction
    public:
        explicit HadesOsEditor(HadesOs * hadesOs, QWidget * parent = nullptr);
        ~HadesOsEditor();

        //////////
        //  core::ComponentEditor
    public:
        virtual void            refresh() override;

        //////////
        //  Implementation
    private:
        Ui::HadesOsEditor *     _ui;
        HadesOs *               _hadesOs;
        bool                    _refreshUnderway = false;

        //  Helpers
        void                    _refreshSharedHostFoldersList();
        void                    _refreshActionButtons();

        HadesOs::SharedHostFolder   _selectedSharedHostFolder();    //  "invalid" if none

        void                    _addSharedHostFolder();
        void                    _modifySharedHostFolder();
        void                    _removeSharedHostFolder();

        //////////
        //  Signal handlers
    private slots:
        void                    _addSharedHostFolderPushButtonClicked();
        void                    _modifySharedHostFolderPushButtonClicked();
        void                    _removeSharedHostFolderPushButtonClicked();
        void                    _sharedHostFoldersListWidgetCurrentRowChanged(int);
    };
}

//  End of emuone-hadedos/HadesOsEditor.hpp

