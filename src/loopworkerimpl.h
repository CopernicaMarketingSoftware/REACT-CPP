/**
 *  LoopWorkerImpl.h
 *
 *  Worker that executes code in the scope of the main loop
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React {

/**
 *  Class definition
 */
class LoopWorkerImpl : public WorkerImpl
{
    /**
     *  The watcher to synchronize with the main thread
     *  @var    SynchronizeWatcher
     */
    SynchronizeWatcher _watcher;

    /**
     *  The callbacks to execute from the main thread
     */
    std::deque<std::function<void()>> _callbacks;

    /**
     *  Mutex to protect the callback list
     */
    std::mutex _mutex;

    /**
     *  Callback function that is executed from the loop context
     */
    void run();

public:
    /**
     *  Construct the loop worker
     *
     *  @param  loop    loop to execute code in
     */
    LoopWorkerImpl(Loop *loop);

    /**
     *  Execute a function
     *
     *  @param  function    the code to execute
     */
    virtual void execute(const std::function<void()> &callback);

};

/**
 *  End namespace
 */
}
