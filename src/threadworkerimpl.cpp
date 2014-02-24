/**
 *  ThreadWorkerImpl.cpp
 *
 *  The worker that executes codes in another thread.
 *
 *  @copyright 2014 Copernica BV
 */

#include "includes.h"

/**
 *  Set up namespace
 */
namespace React {

/**
 *  Construct the thread worker
 */
ThreadWorkerImpl::ThreadWorkerImpl() :
    _thread(&ThreadWorkerImpl::run, this)
{}

/**
 *  Clean up the worker
 */
ThreadWorkerImpl::~ThreadWorkerImpl()
{
    // lock the mutex
    _mutex.lock();

    // we should no longer be running
    _running = false;

    // unlock the mutex
    _mutex.unlock();

    // signal thread to stop running
    _condition.notify_one();

    // and join the thread
    _thread.join();
}

/**
 *  Callback function that is executed from the loop context
 */
void ThreadWorkerImpl::run()
{
    // keep going until we are signalled to stop
    while (true)
    {
        // lock the callback list
        std::unique_lock<std::mutex> lock(_mutex);

        // wait for new work to arrive or the worker to be shutdown
        while (_callbacks.empty() && _running) _condition.wait(lock);

        // are we no longer supposed to be running?
        if (!_running) return;

        // retrieve the callback
        auto callback = _callbacks.front();
        _callbacks.pop_front();

        // release the lock
        lock.unlock();

        // run the callback
        callback();
    }
}

/**
 *  Execute a function
 *
 *  @param  function    the code to execute
 */
void ThreadWorkerImpl::execute(const std::function<void()> &callback)
{
    // lock the callback list
    std::unique_lock<std::mutex> lock(_mutex);

    // push the callback onto the list
    _callbacks.push_back(callback);

    // if there is more than one callback remaining, the thread will not be waiting
    if (_callbacks.size() != 1) return;

    // signal the worker thread to pick up the work
    _condition.notify_one();
}

/**
 *  End namespace
 */
}

