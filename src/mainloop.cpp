/**
 *  MainLoop.cpp
 *
 *  Mainloop implementation
 *
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace React {

/**
 *  Register a function that is called the moment a signal is fired.
 *  @param  signum      The signal
 *  @param  callback    Function that is called the moment the signal is caught
 *  @return             Object that can be used to stop checking for signals
 */
std::shared_ptr<Signal> MainLoop::onSignal(int signum, const SignalCallback &callback)
{
    // create self-destructing implementation object
    auto *signal = new SharedSignal(this, signum, callback);
    
    // done
    return signal->pointer();
}

/**
 *  End namespace
 */
}

