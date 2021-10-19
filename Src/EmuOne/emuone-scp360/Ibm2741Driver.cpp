//
//  emuone-scp360/Ibm2741Driver.cpp
//
//  The scp360::Ibm2741Driver class implementation
//
//////////
#include "emuone-scp360/API.hpp"
using namespace scp360;

//////////
//  Construction/destruction
Ibm2741Driver::Ibm2741Driver()
{
}

Ibm2741Driver::~Ibm2741Driver()
{
}

//////////
//  DeviceDriver
scp360::Device::Flags Ibm2741Driver::deviceFlags() const
{
    return Device::Flags::WriteBlock;
}

ErrorCode Ibm2741Driver::initialiseDevice(Device * device, IoCompletionListener * ioCompletionListener)
{
    Q_ASSERT(device != nullptr);
    Q_ASSERT(ioCompletionListener != nullptr);

    ibm360::Ibm2741 * ibm2741 = _resolveIbm2741(device);
    if (ibm2741 == nullptr)
    {
        return ErrorCode::ERR_PAR;
    }

    if (!_knownHardwareDevices.contains(ibm2741))
    {   //  Add & set up
        _knownHardwareDevices.insert(ibm2741);
        _ibm2741TransferCompletionListeners.insert(new _Ibm2741TransferCompletionListener(this, device));
        _ibm2741IoCompletionListeners.insert(new _Ibm2741IoCompletionListener(this, device));
    }
    device->setState(Device::State::Ready);

    //  Send "initialise" instruction to IBM 2741
    _Ibm2741IoCompletionListener * ibm2741IoCompletionListener = nullptr;
    for (auto l : _ibm2741IoCompletionListeners)
    {   //  TODO spped up look-up!
        if (l->_device == device)
        {   //  This one
            ibm2741IoCompletionListener = l;
            break;
        }
    }
    ibm2741IoCompletionListener->ioCompletionListener = ioCompletionListener;
    ibm360::Ibm2741::ErrorCode ibm2741ErrorCode = ibm2741->beginReset(ibm2741IoCompletionListener);
    if (ibm2741ErrorCode != ibm360::Ibm2741::ErrorCode::Success)
    {   //  OOPS! Cound not start resetting!
        return _translateErrorCode(ibm2741ErrorCode);
    }

    //  Reset started - eventially device will notify the completion listener
    return ErrorCode::ERR_OK;
}

ErrorCode Ibm2741Driver::deinitialiseDevice(Device * device, IoCompletionListener * ioCompletionListener)
{
    Q_ASSERT(device != nullptr);
    Q_ASSERT(ioCompletionListener != nullptr);

    ibm360::Ibm2741 * ibm2741 = _resolveIbm2741(device);
    if (ibm2741 == nullptr)
    {
        return ErrorCode::ERR_PAR;
    }

    //  Un-setup the device
    _Ibm2741IoCompletionListener * ibm2741IoCompletionListener = nullptr;
    for (auto l : _ibm2741IoCompletionListeners)
    {   //  TODO spped up look-up!
        if (l->_device == device)
        {   //  This one
            ibm2741IoCompletionListener = l;
            break;
        }
    }
    _ibm2741IoCompletionListeners.remove(ibm2741IoCompletionListener);
    delete ibm2741IoCompletionListener;

    _Ibm2741TransferCompletionListener * ibm2741TransferCompletionListener = nullptr;
    for (auto l : _ibm2741TransferCompletionListeners)
    {   //  TODO spped up look-up!
        if (l->_device == device)
        {   //  This one
            ibm2741TransferCompletionListener = l;
            break;
        }
    }
    _ibm2741TransferCompletionListeners.remove(ibm2741TransferCompletionListener);
    delete ibm2741TransferCompletionListener;

    _knownHardwareDevices.remove(ibm2741);

    device->setState(Device::State::Unknown);

    return ErrorCode::ERR_SUP;
}

ErrorCode Ibm2741Driver::writeBlock(Device * device, const util::Buffer * buffer, TransferCompletionListener * transferCompletionListener)
{
    Q_ASSERT(device != nullptr);
    Q_ASSERT(buffer != nullptr);
    Q_ASSERT(transferCompletionListener != nullptr);

    ibm360::Ibm2741 * ibm2741 = _resolveIbm2741(device);
    if (ibm2741 == nullptr)
    {
        return ErrorCode::ERR_PAR;
    }

    //  Ask the IBM 2741 to begin writing
    _Ibm2741TransferCompletionListener * ibm2741TransferCompletionListener = nullptr;
    for (auto l : _ibm2741TransferCompletionListeners)
    {   //  TODO speed up look-up!
        if (l->_device == device)
        {   //  This one
            ibm2741TransferCompletionListener = l;
            break;
        }
    }
    ibm2741TransferCompletionListener->transferCompletionListener = transferCompletionListener;
    ibm360::Ibm2741::ErrorCode ibm2741ErrorCode = ibm2741->beginWriteBlock(buffer, ibm2741TransferCompletionListener);
    if (ibm2741ErrorCode != ibm360::Ibm2741::ErrorCode::Success)
    {   //  OOPS! Cound not start writing!
        return _translateErrorCode(ibm2741ErrorCode);
    }

    //  Writing started - eventially device will notify the completion listener
    return ErrorCode::ERR_OK;
}

ErrorCode Ibm2741Driver::haltIo(Device * device)
{
    ibm360::Ibm2741 * ibm2741 = _resolveIbm2741(device);
    if (ibm2741 == nullptr)
    {
        return ErrorCode::ERR_PAR;
    }

    return _translateErrorCode(ibm2741->haltIo());
}

ErrorCode Ibm2741Driver::validateOpenFlags(OpenFileFlags openFlags) const
{
    //  IBM 2741 only allows "fixed unblocked" R/W in text (sequential) mode
    if ((openFlags & OpenFileFlags::RecordsMask) == OpenFileFlags::FixedUnblockedRecords &&
        ((openFlags & OpenFileFlags::DirectionMask) == OpenFileFlags::ReadOnly ||
         (openFlags & OpenFileFlags::DirectionMask) == OpenFileFlags::WriteOnly ||
         (openFlags & OpenFileFlags::DirectionMask) == OpenFileFlags::ReadWrite) &&
        (openFlags & OpenFileFlags::ModeMask) == OpenFileFlags::TextMode &&
        (openFlags & OpenFileFlags::AccessMask) == OpenFileFlags::SequentialAccess)
    {
        return ErrorCode::ERR_OK;
    }
    return ErrorCode::ERR_SUP;
}

//////////
//  Implementation helpers
ibm360::Ibm2741 * Ibm2741Driver::_resolveIbm2741(Device * device)
{
    PhysicalDevice * physicalDevice = dynamic_cast<PhysicalDevice*>(device);
    if (physicalDevice == nullptr)
    {
        return nullptr;
    }
    return dynamic_cast<ibm360::Ibm2741*>(physicalDevice->hardwareDevice());
}

ErrorCode Ibm2741Driver::_translateErrorCode(ibm360::Ibm2741::ErrorCode ibm2741ErrorCode)
{
    switch (ibm2741ErrorCode)
    {
        case ibm360::Ibm2741::ErrorCode::Success:
            return ErrorCode::ERR_OK;
        case ibm360::Ibm2741::ErrorCode::Busy:
            return ErrorCode::ERR_USE;
        case ibm360::Ibm2741::ErrorCode::Interrupted:
            return ErrorCode::ERR_CAN;
        case ibm360::Ibm2741::ErrorCode::Unknown:
            return ErrorCode::ERR_UNK;
        default:
            Q_ASSERT(false);
    }
}

//////////
//  Ibm2741Driver::_Ibm2741IoCompletionListener
void Ibm2741Driver::_Ibm2741IoCompletionListener::ioCompleted(Ibm2741::ErrorCode errorCode)
{
    Q_ASSERT(ioCompletionListener != nullptr);
    ioCompletionListener->ioCompleted(_device, _translateErrorCode(errorCode));
}

//////////
//  Ibm2741Driver::_Ibm2741TransferCompletionListener
void Ibm2741Driver::_Ibm2741TransferCompletionListener::transferCompleted(uint32_t bytesTransferred, Ibm2741::ErrorCode errorCode)
{
    Q_ASSERT(transferCompletionListener != nullptr);
    transferCompletionListener->transferCompleted(_device, bytesTransferred, _translateErrorCode(errorCode));
}

//  End of emuone-scp360/Ibm2741Driver.cpp
