//
//  emuone-hades/kernel/SystemCalls.cpp - emuone::hades::kernel::SystemCalls class implementation
//
//  EmuOne
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////
#include "emuone-hades/API.hpp"
using namespace emuone::hades::kernel;

//////////
//  System calls
//  TODO organize into groups
KErrno SystemCalls::kwrite(const QString & s)
{
    emuone::util::Lock _(_runner->_nativeThread->kernel->kernelGuard);

    //  Write to Kernel Console line-by-line
    for (QChar c : s)
    {
        if (c == '\n')
        {   //  flush
            _runner->_nativeThread->kernel->_kernelConsoleContent.append(_kernelConsoleBuffer);
            _kernelConsoleBuffer.clear();
        }
        else
        {   //  Accumulate
            _kernelConsoleBuffer += c;
        }
    }
    return K_EOK;
}

KErrno SystemCalls::signal(int sig, kernel::SignalDisposition handler)
{
    emuone::util::Lock _(_runner->_nativeThread->kernel->kernelGuard);

    auto process = dynamic_cast<NativeProcess*>(_runner->_nativeThread->process);
    Q_ASSERT(process != nullptr);

    if (sig > 0 && sig < kernel::K_NSIG)
    {
        switch (handler)
        {
            case kernel::K_SIG_TRM:
            case kernel::K_SIG_IGN:
            case kernel::K_SIG_TRD:
            case kernel::K_SIG_STP:
            case kernel::K_SIG_CNT:
                process->signalDispositions[sig] = handler;
                break;
            case kernel::K_SIG_DFL:
                process->signalDispositions[sig] = process->defaultSignalDispositions[sig];
                break;
            default:
                //  Be defensive in release mode
                break;
        }
    }
    return K_EOK;
}

KErrno SystemCalls::signal(int sig, kernel::NativeSignalHandler handler)
{
    emuone::util::Lock _(_runner->_nativeThread->kernel->kernelGuard);

    auto process = dynamic_cast<NativeProcess*>(_runner->_nativeThread->process);
    Q_ASSERT(process != nullptr);

    if (sig > 0 && sig < kernel::K_NSIG)
    {
        if (handler != nullptr)
        {   //  Use the "handler"
            process->signalDispositions[sig] = kernel::K_SIG_UDF;
            process->nativeSignalHandlers[sig] = handler;
        }
        else
        {   //  Revert to default
            process->signalDispositions[sig] = process->defaultSignalDispositions[sig];
        }
    }
    return K_EOK;
}

KErrno SystemCalls::getAtom(const QString & value, Atom & atom)
{
    emuone::util::Lock _(_runner->_nativeThread->kernel->kernelGuard);
    _processPendngSignals();

    kernel::Atom * a = nullptr;
    KErrno err =
        _runner->_nativeThread->kernel->getAtom(
            _runner->_nativeThread->process,
            value,
            a);
    if (err != kernel::KErrno::K_EOK)
    {
        return err;
    }
    atom.oid = uint32_t(a->oid);
    return kernel::KErrno::K_EOK;
}

KErrno SystemCalls::releaseAtom(Atom atom)
{
    emuone::util::Lock _(_runner->_nativeThread->kernel->kernelGuard);
    _processPendngSignals();

    kernel::Oid oid {atom.oid};
    auto a =
        dynamic_cast<kernel::Atom*>(
            _runner->_nativeThread->kernel->_objects.value(oid, nullptr));
    if (a == nullptr ||
        !_runner->_nativeThread->process->interestingAtoms.contains(a))
    {   //  OOPS! No such Atom or current Process not interested
        return K_EINVAL;
    }
    return _runner->_nativeThread->kernel->releaseAtom(
                _runner->_nativeThread->process,
                a);
}

KErrno SystemCalls::yield()
{
    emuone::util::Lock _(_runner->_nativeThread->kernel->kernelGuard);
    _processPendngSignals();

    QThread::yieldCurrentThread();
    return K_EOK;
}

//////////
//  Implementation helpers
void SystemCalls::_processPendngSignals()
{
    Q_ASSERT(_runner->_nativeThread->kernel->kernelGuard.isLockedByCurrentThread());

    //  Some signals cannot be ignored or masked
    auto thread = _runner->_nativeThread;
    auto process = dynamic_cast<NativeProcess*>(thread->process);
    Q_ASSERT(process != nullptr);

    //  Sone signals are special
    if (process->pendingSignals & K_sigmask(K_SIGKILL))
    {   //  There's no way to drop a pending SIGKILL
        throw uint32_t(128 + K_SIGKILL);    //  force thread exit
    }
    //  Other signals can be masked
    if (K_sigset_t sigs = process->pendingSignals & (~thread->signalMask))
    {   //  At least one signal is a) pending and b) NOT masked for this thread
        int sig = std::countr_zero(sigs);
        //  Mark the signal as "handled"
        process->pendingSignals &= ~K_sigmask(sig);
        //  How do we process the signal ?
        switch (process->signalDispositions[sig])
        {
            case K_SIG_IGN: //  Do nothing
                break;
            case K_SIG_TRM: //  Terminate the process
            case K_SIG_TRD: //  TODO properly!
                throw uint32_t(128 + sig);
            case K_SIG_STP: //  TODO implement
                Q_ASSERT(false);
                break;
            case K_SIG_CNT: //  TODO implement
                Q_ASSERT(false);
                break;
            //  Special values for signal() function
            case K_SIG_UDF:
                process->nativeSignalHandlers[sig](sig);
                break;
            default:    //  Be defensive in release mode
            case K_SIG_DFL:
                break;
        }
    }

    //  A QThread's interruption can be requected ONLY
    //  when the Kernel wants that QThread to exit ASAP
    Q_ASSERT(thread->_runnerThread == QThread::currentThread());
    if (thread->_runnerThread->isInterruptionRequested())
    {   //  Terminate thread voluntarily...
        throw uint32_t(128 + K_SIGKILL);   //  ...as if by a SIGKILL
    }
}

//  End of emuone-hades/kernel/SystemCalls.cpp
