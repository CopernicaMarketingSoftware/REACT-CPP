/**
 *  ThreadWorkerImpl.h
 *
 *  The worker that executes codes in another thread.
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
class ThreadWorkerImpl : public WorkerImpl
{
private:
    /**
     *  The thread we run
     */
    std::thread _thread;

    /**
     *  Are we still running?
     */
    bool _running;

    /**
     *  The callbacks to execute from the main thread
     */
    std::deque<std::function<void()>> _callbacks;

    /**
     *  Mutex to protect the callback list
     */
    std::mutex _mutex;

    /**
     *  Condition to signal arrival of new work
     */
    std::condition_variable _condition;

    /**
     *  Callback function that is executed from the loop context
     */
    void run();
public:
    /**
     *  Construct the thread worker
     */
    ThreadWorkerImpl();

    /**
     *  Clean up the workers
     */
    virtual ~ThreadWorkerImpl();

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

