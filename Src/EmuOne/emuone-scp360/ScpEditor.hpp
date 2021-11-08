//
//  emuone-scp360/ScpEditor.hpp
//
//  "SCP editor" control
//
//////////
#pragma once
#include "emuone-scp360/API.hpp"
#include <QWidget>

namespace scp360
{
    namespace Ui { class ScpEditor; }

    //////////
    //  Configuration editor for SCP component
    class ScpEditor : public core::ComponentEditor
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ScpEditor)

        //////////
        //  Construction/destruction
    public:
        explicit ScpEditor(scp360::Scp * scp, QWidget * parent = nullptr);
        virtual ~ScpEditor();

        //////////
        //  ComponentEditor
    public:
        virtual void            refresh() override;

        //////////
        //  Implementation
    private:
        Ui::ScpEditor *         _ui;
        scp360::Scp *const      _scp;
        bool                    _refreshUnderway = false;

        //  Helpers
        void                    _refreshSharedFoldersList();
        scp360::Scp::SharedFolder * _selectedSharedFolder();
        void                    _setSelectedSharedFolder(scp360::Scp::SharedFolder * sharedFolder);

        //////////
        //  Event handlers
    private slots:
        void                    _sharedFoldersTreeWidgetItemSelectionChanged();
        void                    _addSharedFolderPushButtonClicked();
        void                    _removeSharedFolderPushButtonClicked();
        void                    _modifySharedFolderPushButtonClicked();
    };
}

//  End of emuone-scp360/ScpEditor.hpp

