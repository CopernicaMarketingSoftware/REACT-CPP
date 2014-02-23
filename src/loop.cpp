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
std::shared_ptr<Reader> Loop::onReadable(int fd, const ReadCallback &callback)
{
    // create self-destructing implementation object
    auto *reader = new SharedReader(this, fd, callback);
    
    // done
    return reader->pointer();
}

/**
 *  Register a function that is called the moment a filedescriptor becomes writable.
 *  @param  fd          The filedescriptor
 *  @param  callback    Function that is called the moment the fd is readable
 *  @return             Object that can be used to stop checking for writability
 */
std::shared_ptr<Writer> Loop::onWritable(int fd, const WriteCallback &callback)
{
    // create self-destructing implementation object
    auto *writer = new SharedWriter(this, fd, callback);
    
    // done
    return writer->pointer();
}

/**
 *  Register a timeout to be called in a certain amount of time
 *  @param  timeout     The timeout in seconds
 *  @param  callback    Function that is called when the timer expires
 *  @return             Object that can be used to stop or edit the timer
 */
std::shared_ptr<Timer> Loop::onTimeout(Timestamp timeout, const TimerCallback &callback)
{
    // create self-destructing implementation object
    auto *timer = new SharedTimer(this, timeout, callback);
    
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
std::shared_ptr<Interval> Loop::onInterval(Timestamp initial, Timestamp timeout, const IntervalCallback &callback)
{
    // create self-destructing implementation object
    auto *interval = new SharedInterval(this, initial, timeout, callback);
    
    // done
    return interval->pointer();
}

/**
 *  Register a synchronize function
 *  @param  callback    The callback that is called 
 *  @return             Object that can be used to stop watching, or to synchronize
 */
std::shared_ptr<Synchronizer> Loop::onSynchronize(const SynchronizeCallback &callback)
{
    // create implementation object
    auto *synchronizer = new Synchronizer(this, callback);
    
    // done, wrap in a shared pointer
    return std::shared_ptr<Synchronizer>(synchronizer);
}

/**
 *  End namespace
 */
}

