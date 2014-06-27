/**
 *  Loop.cpp
 *
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace React {

/**
 *  Register a function that is called the moment a filedescriptor becomes readable
 *  @param  fd          The filedescriptor
 *  @param  callback    Function that is called the moment the fd is readable
 *  @return             Object that can be used to stop checking for readability
 */
std::shared_ptr<ReadWatcher> Loop::onReadable(int fd, const ReadCallback &callback)
{
    // check if callback is valid
    if (!callback) return nullptr;

    // create self-destructing implementation object
    auto *reader = new SharedReadWatcher(this, fd, callback);

    // done
    return reader->pointer();
}

/**
 *  Register a function that is called the moment a filedescriptor becomes writable.
 *  @param  fd          The filedescriptor
 *  @param  callback    Function that is called the moment the fd is readable
 *  @return             Object that can be used to stop checking for writability
 */
std::shared_ptr<WriteWatcher> Loop::onWritable(int fd, const WriteCallback &callback)
{
    // check if callback is valid
    if (!callback) return nullptr;

    // create self-destructing implementation object
    auto *writer = new SharedWriteWatcher(this, fd, callback);

    // done
    return writer->pointer();
}

/**
 *  Register a timeout to be called in a certain amount of time
 *  @param  timeout     The timeout in seconds
 *  @param  callback    Function that is called when the timer expires
 *  @return             Object that can be used to stop or edit the timer
 */
std::shared_ptr<TimeoutWatcher> Loop::onTimeout(Timestamp timeout, const TimeoutCallback &callback)
{
    // check if callback is valid
    if (!callback) return nullptr;

    // create self-destructing implementation object
    auto *timer = new SharedTimeoutWatcher(this, timeout, callback);

    // done
    return timer->pointer();
}

/**
 *  Register a function to be called periodically at fixed intervals
 *  @param  initial     Initial interval time
 *  @param  timeout     The interval in seconds
 *  @param  callback    Function that is called when the timer expires
 *  @return             Object that can be used to stop or edit the interval
 */
std::shared_ptr<IntervalWatcher> Loop::onInterval(Timestamp initial, Timestamp timeout, const IntervalCallback &callback)
{
    // check if callback is valid
    if (!callback) return nullptr;

    // create self-destructing implementation object
    auto *interval = new SharedIntervalWatcher(this, initial, timeout, callback);

    // done
    return interval->pointer();
}

/**
 *  Register a synchronize function
 *  @param  callback    The callback that is called
 *  @return             Object that can be used to stop watching, or to synchronize
 */
std::shared_ptr<SynchronizeWatcher> Loop::onSynchronize(const SynchronizeCallback &callback)
{
    // check if callback is valid
    if (!callback) return nullptr;

    // create the watcher
    return std::make_shared<SynchronizeWatcher>(this, callback);
}

/**
 *  Register a cleanup function
 *
 *  This method takes a callback to be executed right before the event loop
 *  gets destroyed.
 *
 *  @param  callback    Function to invoke before destroying the loop
 */
std::shared_ptr<CleanupWatcher> Loop::onCleanup(const CleanupCallback &callback)
{
    // check if callback is valid
    if (!callback) return nullptr;

    // create self-destructing implementation object
    auto *cleanup = new SharedCleanupWatcher(this, callback);

    // done
    return cleanup->pointer();
}

/**
 *  End namespace
 */
}

