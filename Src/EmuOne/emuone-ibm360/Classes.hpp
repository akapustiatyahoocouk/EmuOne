//
//  emuone-ibm360/Classes.hpp
//
//  Forward declarations and typedefs
//
//////////
#pragma once

//  QT does not play well with "form" classes
class EMUONE_IBM360_EXPORT Ibm360StorageEditor;
class EMUONE_IBM360_EXPORT Ibm360ProcessorEditor;

class EMUONE_IBM360_EXPORT Ibm2741Editor;
class EMUONE_IBM360_EXPORT Ibm2741FullScreenWidget;

namespace ibm360
{
    using Ibm2741EditorList = QList<Ibm2741Editor*>;
}

//  End of emuone-ibm360/Classes.hpp
