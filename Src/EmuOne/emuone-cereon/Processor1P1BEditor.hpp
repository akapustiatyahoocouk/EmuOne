//
//  emuone-cereon/Processor1P1BEditor.hpp
//
//  Cereon-1P1B unit editor
//
//////////
#pragma once
#include "emuone-cereon/API.hpp"
#include <QWidget>

namespace cereon {

    namespace Ui { class Processor1P1BEditor; }

    //////////
    //  Cereon-1P1B unit editor
    class EMUONE_CEREON_EXPORT Processor1P1BEditor : public core::ComponentEditor
    {
        Q_OBJECT

        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Processor1P1BEditor)

        //////////
        //  Construction/destruction
    public:
        explicit Processor1P1BEditor(Processor1P1B * processor, QWidget * parent = nullptr);
        virtual ~Processor1P1BEditor();

        //////////
        //  core::ComponentEditor
    public:
        virtual void            refresh() override;

        //////////
        //  Implementation
    private:
        Ui::Processor1P1BEditor *   _ui;
        Processor1P1B *             _processor;
        bool                        _refreshUnderway = false;

        //  Helpers
        void                        _applyClockCurrencyChanges();
        void                        _applyByteOrderChanges();
        void                        _applyProcessorIdChanges();
        void                        _applyIsPrimaryChanges();
        void                        _applyBootstrapIpChanges();

        //////////
        //  Event handlers
    private slots:
        void                        _clockFrequencyValueLineEditTextChanged(const QString &);
        void                        _clockFrequencyUnitComboBoxCurrentIndexChanged(int);
        void                        _byteOrderComboBoxCurrentIndexChanged(int);
        void                        _processorIdLineEditTextChanged(const QString &);
        void                        _primaryCheckBoxStateChanged(int);
        void                        _bootstrapIpLineEditTextChanged(const QString &);
    };
}

//  Ed of emuone-cereon/Processor1P1BEditor.hpp

