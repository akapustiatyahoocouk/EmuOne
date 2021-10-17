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

    PhysicalDevice * physicalDevice = dynamic_cast<PhysicalDevice*>(device);
    if (physicalDevice == nullptr)
    {
        return ErrorCode::ERR_PAR;
    }
    ibm360::Ibm2741 * ibm2741 = dynamic_cast<ibm360::Ibm2741*>(physicalDevice->hardwareDevice());
    if (ibm2741 == nullptr)
    {
        return ErrorCode::ERR_PAR;
    }

    if (!_knownHardwareDevices.contains(ibm2741))
    {   //  Add & set up
        _knownHardwareDevices.insert(ibm2741);
        _transferCompletionListeners.insert(new _TransferCompletionListener(this, physicalDevice));
    }
    device->setState(Device::State::Ready);

    return ErrorCode::ERR_SUP;
}

ErrorCode Ibm2741Driver::deinitialiseDevice(Device * device, IoCompletionListener * ioCompletionListener)
{
    Q_ASSERT(device != nullptr);
    Q_ASSERT(ioCompletionListener != nullptr);

    PhysicalDevice * physicalDevice = dynamic_cast<PhysicalDevice*>(device);
    if (physicalDevice == nullptr)
    {
        return ErrorCode::ERR_PAR;
    }
    ibm360::Ibm2741 * ibm2741 = dynamic_cast<ibm360::Ibm2741*>(physicalDevice->hardwareDevice());
    if (ibm2741 == nullptr || !_knownHardwareDevices.contains(ibm2741))
    {
        return ErrorCode::ERR_PAR;
    }

    //  Un-setup the device
    _TransferCompletionListener * transferCompletionListener = nullptr;
    for (auto l : _transferCompletionListeners)
    {   //  TODO spped up look-up!
        if (l->_physicalDevice == physicalDevice)
        {   //  This one
            transferCompletionListener = l;
            break;
        }
    }
    _transferCompletionListeners.remove(transferCompletionListener);
    delete transferCompletionListener;
    _knownHardwareDevices.remove(ibm2741);

    device->setState(Device::State::Unknown);

    return ErrorCode::ERR_SUP;
}

ErrorCode Ibm2741Driver::writeBlock(Device * device, const util::Buffer * buffer, TransferCompletionListener * transferCompletionListener)
{
    Q_ASSERT(device != nullptr);
    Q_ASSERT(buffer != nullptr);
    Q_ASSERT(transferCompletionListener != nullptr);

    PhysicalDevice * physicalDevice = dynamic_cast<PhysicalDevice*>(device);
    if (physicalDevice == nullptr)
    {
        return ErrorCode::ERR_PAR;
    }
    ibm360::Ibm2741 * ibm2741 = dynamic_cast<ibm360::Ibm2741*>(physicalDevice->hardwareDevice());
    if (ibm2741 == nullptr || !_knownHardwareDevices.contains(ibm2741))
    {
        return ErrorCode::ERR_PAR;
    }

    //  Ask the IBM 2741 to begin writing
    _TransferCompletionListener * ibm2741TransferCompletionListener = nullptr;
    for (auto l : _transferCompletionListeners)
    {   //  TODO spped up look-up!
        if (l->_physicalDevice == physicalDevice)
        {   //  This one
            ibm2741TransferCompletionListener = l;
            break;
        }
    }
    ibm2741TransferCompletionListener->transferCompletionListener = transferCompletionListener;
    ibm360::Ibm2741::ErrorCode ibm2741ErrorCode = ibm2741->beginWrite(buffer, ibm2741TransferCompletionListener);
    if (ibm2741ErrorCode != ibm360::Ibm2741::ErrorCode::Success)
    {   //  OOPS! Cound not start writing!
        return _translateErrorCode(ibm2741ErrorCode);
    }

    //  Writing started - eventially device will notify the completion listener
    return ErrorCode::ERR_OK;
}

//////////
//  Implementation helpers
ErrorCode Ibm2741Driver::_translateErrorCode(ibm360::Ibm2741::ErrorCode ibm2741ErrorCode)
{
    switch (ibm2741ErrorCode)
    {
        case ibm360::Ibm2741::ErrorCode::Success:
            return ErrorCode::ERR_OK;
        case ibm360::Ibm2741::ErrorCode::Busy:
            return ErrorCode::ERR_USE;
        case ibm360::Ibm2741::ErrorCode::Unknown:
            return ErrorCode::ERR_UNK;
        default:
            Q_ASSERT(false);
    }
}

//////////
//  Ibm2741Driver::_Ibm2741TransferCompletionListener
void Ibm2741Driver::_TransferCompletionListener::transferCompleted(uint32_t bytesTransferred, Ibm2741::ErrorCode errorCode)
{
    Q_ASSERT(transferCompletionListener != nullptr);
    transferCompletionListener->transferCompleted(_physicalDevice, bytesTransferred, _translateErrorCode(errorCode));
}

//  End of emuone-scp360/Ibm2741Driver.cpp
