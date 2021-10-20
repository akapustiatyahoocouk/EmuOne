//
//  emuone-scp360/Types.hpp
//
//  SCP/360 basic types
//
//////////
#pragma once

namespace scp360
{
    //  Error codes used by SCP/360 and user processes
    enum class ErrorCode : uint32_t
    {
        ERR_OK  = 0x00000000,   //  SUCCESS
        ERR_SVC = 0x00000004,   //  UNKNOWN SVC
        ERR_KFL = 0x00000008,   //  KERNEL HEAP FULL
        ERR_SFL = 0x0000000C,   //  STORAGE FULL
        ERR_LIM = 0x00000010,   //  LIMIT REACHED
        ERR_PAR = 0x00000014,   //  INVALID PARAMETER
        ERR_USE = 0x00000018,   //  STILL IN USE
        ERR_IO  = 0x0000001C,   //  I/O ERROR
        ERR_SUP = 0x00000020,   //  OPERATION NOT SUPPORTED
        ERR_EOF = 0x00000024,   //  EOF IN INPUT, MEDIUM FULL ON OUTPUT
        ERR_NOF = 0x00000028,   //  OBJECT (E.G. FILE) NOT FOUND
        ERR_EXS = 0x0000002C,   //  OBJECT (E.G. VOLUME) ALREADY EXISTS
        ERR_BTS = 0x00000030,   //  RECIPIENT BUFFER TOO SMALL FOR THE VALUE
        ERR_CAN = 0x00000034,   //  SERVICE CANCELLED BEFORE IT COULD COMPLETE
        ERR_BAD = 0x00000038,   //  BAD DATA
        ERR_ACC = 0x0000003C,   //  ACCESS DENIED
        ERR_UNK = 0x00000040,   //  UNKNOWN ERROR, ALWAYS HAS THE MAXIMUM VALUE
        ERR_MAX = ERR_UNK       //  THE MAXIMUM VALID ERROR CODE VALUE
    };

    //  System call codes
    enum class SystemCallCode : uint32_t
    {
        //  I/O
        ReadFromOperator        = 0x00010001,   //  READ FROM OPERATOR
        WriteToOperator         = 0x00010002,   //  WRITE TO OPERATOR
        OpenFile                = 0x00010003,   //  OPEN FILE
        CloseFile               = 0x00010004,   //  CLOSE FILE
        WriteToFile             = 0x00010005,   //  WRITE TO FILE
        ReadFromFile            = 0x00010006,   //  READ FROM FILE
        FileInformation         = 0x00010007,   //  FILE INFORMATION
        VolumeInformation       = 0x00010008,   //  VOLUME INFORMATION
        NamedFileInformation    = 0x00010009,   //  NAMED FILE INFORMATION

        DeleteNamedFile         = 0x0001000B,   //  DELETE NAMED FILE

        BeginVolumeScan         = 0x0001000D,   //  BEGIN VOLUME SCAN
        ContinueVolumeScan      = 0x0001000E,   //  CONTINUE VOLUME SCAN
        EndVolumeScan           = 0x0001000F,   //  END VOLUME SCAN

        //  PROCESS MANAGEMENT
        ExitProcess             = 0x00020001,   //  EXIT PROCESS

        CountEnvironmentVariableValues          = 0x00020101,   //  COUNT ENVIRONMENT VARIABLE VALUES
        GetEnvironmentVariableValueList         = 0x00020102,   //  GET ENVIRONMENT VARIABLE VALUE (LIST)
        GetEnvironmentVariableValueValueScalar  = 0x00020103,   //  GET ENVIRONMENT VARIABLE VALUE (SCALAR)
        GetEnvironmentVariableValueValueList    = 0x00020104,   //  SET ENVIRONMENT VARIABLE VALUE (LIST)
        SetEnvironmentVariableValueValueScalar  = 0x00020105,   //  SET ENVIRONMENT VARIABLE VALUE (SCALAR)
        SetEnvironmentVariableValueValueList    = 0x00020106,   //  ADD TO ENVIRONMENT VARIABLE VALUE (LIST)
        DeleteEnvironmentVariable               = 0x00020107,   //  DELETE ENVIRONMENT VARIABLE

        GetCurrentPid           = 0x00020201,   //  GET CURRENT PID

        //  STORAGE MANAGEMENT
        CreateAndMapSegment     = 0x00030001,   //  CREATE & MAP SEGMENT
    };

    //  A "resource handle" is a value that allows a running Process to refer to a Resource
    typedef uint16_t ResourceHandle;

    //  "Open file" flags
    enum class OpenFileFlags : uint16_t
    {
        FixedUnblockedRecords       = 0x0001,   //  FIXED UNBLOCKED RECORD FORMAT
        FixedBlockedRecords         = 0x0002,   //  FIXED BLOCKED RECORD FORMAT
        VariableUnblockedRecords    = 0x0003,   //  VARIABLE UNBLOCKED RECORD FORMAT
        VariableBlockedRecords      = 0x0004,   //  VARIABLE BLOCKED RECORD FORMAT
        UndefinedRecords            = 0x0005,   //  UNDEFINED RECORD FORMAT
        RecordsMask                 = 0x000F,

        ReadOnly                    = 0x0010,   //  OPEN FILE FOR INPUT
        WriteOnly                   = 0x0020,   //  OPEN FILE FOR OUTPUT
        ReadWrite                   = 0x0030,   //  OPEN FILE FOR INPUT AND OUTPUT (UPDATE)
        DirectionMask               = 0x00F0,

        BinaryMode                  = 0x0100,   //  OPEN FILE FOR BINARY I/O
        TextMode                    = 0x0200,   //  OPEN FILE FOR TEXTUAL I/O
        ModeMask                    = 0x0F00,

        SequentialAccess            = 0x1000,   //  OPEN FILE FOR SEQUENTIAL ACCESS
        IndexedSeqentialAccess      = 0x2000,   //  OPEN FILE FOR INDEXED-SEQUENTIAL ACCESS
        DirectAccess                = 0x3000,   //  OPEN FILE FOR DIRECT ACCESS
        AccessMask                  = 0xF000
    };

    EMUONE_SCP360_EXPORT OpenFileFlags operator & (OpenFileFlags op1, OpenFileFlags op2);
    EMUONE_SCP360_EXPORT OpenFileFlags operator | (OpenFileFlags op1, OpenFileFlags op2);
    EMUONE_SCP360_EXPORT OpenFileFlags & operator &= (OpenFileFlags & op1, OpenFileFlags op2);
    EMUONE_SCP360_EXPORT OpenFileFlags & operator |= (OpenFileFlags & op1, OpenFileFlags op2);
}

//  End of emuone-scp360/Types.hpp
