//
//  emuone-scp360/ErrorCodes.hpp
//
//  SCP/360 error codes
//
//////////
#pragma once

namespace scp360
{
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
        ERR_MAX = 0x00000040    //  THE MAXIMUM VALID ERROR CODE VALUE
    };
}

//  End of emuone-scp360/ErrorCodes.hpp
