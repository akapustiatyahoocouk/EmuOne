//
//  emuone-ibm360/Ibm360ProcessorEditor.hpp
//
//  "IBM/360 processorm editor" control
//
//////////
#pragma once
#include "emuone-ibm360/API.hpp"
#include <QWidget>

namespace ibm360
{
    namespace Ui { class Ibm360ProcessorEditor; }

    //////////
    //  The "IBM/360 processorm editor" control
    class Ibm360ProcessorEditor : public core::ComponentEditor
    {
        Q_OBJECT

        //////////
        //  Construction/destruction
    public:
        explicit Ibm360ProcessorEditor(ibm360::Processor * processor, QWidget * parent = nullptr);
        virtual ~Ibm360ProcessorEditor();

        //////////
        //  ComponentEditor
    public:
        virtual void                refresh() override;

        //////////
        //  Implementation
    private:
        Ui::Ibm360ProcessorEditor * _ui;
        ibm360::Processor *         _processor;
        bool                        _refreshUnderway = false;

        //////////
        //  Event handlers
    private slots:
        void                        _clockFrequencyLineEditTextChanged(const QString &);
        void                        _clockFrequencyUnitComboBoxCurrentIndexChanged(int);
        void                        _fetchProtectionCheckBoxClicked();
        void                        _storeProtectionCheckBoxClicked();
        void                        _decimalCheckBoxClicked();
        void                        _floatingPointCheckBoxClicked();
        void                        _byteOrientedOperandCheckBoxClicked();
        void                        _timerCheckBoxClicked();
        void                        _directControlCheckBoxClicked();
    };
}

//  End of emuone-ibm360/Ibm360ProcessorEditor.hpp
