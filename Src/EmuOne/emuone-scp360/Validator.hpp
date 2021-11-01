//
//  emuone-scp360/Validator.hpp
//
//  The SCP/360 property valdators
//
//////////
#pragma once

namespace scp360
{
    //////////
    //  An agent that can "validate" varius properties of SCP object
    class EMUONE_SCP360_EXPORT Validator
    {
        UTILITY_CLASS(Validator)

        //////////
        //  Operations
    public:
        static bool         isValidSegmentName(const QString & name);
        static bool         isValidProcessName(const QString & name);
        static bool         isValidEnvironmentVariableName(const QString & name);
        static bool         isValidEnvironmentVariableScalarValue(const QString & scalarValue);
        static bool         isValidEnvironmentVariableListValue(const QStringList & listValue);
        static bool         isValidPhysicalDeviceName(const QString & name);
        static bool         isValidLogicalDeviceName(const QString & name);
    };
}

//  End of emuone-scp360/Validator.hpp
