/**
 *  LoopWorkerImpl.cpp
 *
 *  Worker that executes code in the scope of the main loop
 *
 *  @copyright 2014 Copernica BV
 */

#include "includes.h"

/**
 *  Set up namespace
 */
namespace React {

/**
 *  Construct the loop worker
 *
 *  @param  loop    loop to execute code in
 */
LoopWorkerImpl::LoopWorkerImpl(Loop *loop) :
    _watcher(loop, [this]() { run(); })
{}

/**
 *  Callback function that is ran from the loop context
 */
void LoopWorkerImpl::run()
{
    // keep going until all the work is done
    while (true)
    {
        // lock the callback list
        _mutex.lock();

        // are there any callbacks to execute?
        if (_callbacks.empty())
        {
            // unlock the mutex
            _mutex.unlock();

            // and exit the loop
            break;
        }

        // retrieve the first callback
        auto callback = _callbacks.front();
        _callbacks.pop_front();

        // unlock the list
        _mutex.unlock();

        // and execute the callback
        callback();
    }
}

/**
 *  Execute a function
 *
 *  @param  function    the code to execute
 */
void LoopWorkerImpl::execute(const std::function<void()> &callback)
{
    // lock the callback list
    std::unique_lock<std::mutex> lock(_mutex);

    // push the callback onto the list
    _callbacks.push_back(callback);

    // if more elements already exist, the synchronizer is
    // already running or queued, so we do not queue it
    // again to avoid incurring additional overhead
    if (_callbacks.size() != 1) return;

    // execute the synchronize watcher
    _watcher.synchronize();
}

/**
 *  End namespace
 */
}
