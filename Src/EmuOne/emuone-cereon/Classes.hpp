//
//  emuone-cereon/Classes.hpp
//
//  Forward declarations and typedefs
//
//////////
#pragma once

namespace cereon
{
    class EMUONE_CORE_EXPORT Processor;
    class EMUONE_CORE_EXPORT ProcessorCore;

    class EMUONE_CORE_EXPORT RamUnitEditor;
    class EMUONE_CORE_EXPORT RomUnitEditor;
    class EMUONE_CORE_EXPORT Processor1P1BEditor;


    using RamUnitEditorList = QList<RamUnitEditor*>;
    using RomUnitEditorList = QList<RomUnitEditor*>;
    using Processor1P1BEditorList = QList<Processor1P1BEditor*>;
}

//  End of emuone-cereon/Classes.hpp
