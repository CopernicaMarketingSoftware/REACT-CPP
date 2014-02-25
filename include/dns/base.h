/**
 *  Base.h
 *
 *  Inaccessible base class of the resolver that is exclusively used internally
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React { namespace Dns {

/**
 *  Class definition
 */
class Base
{
private:
    /**
     *  Set of readers
     *  @var    std::map
     */
    std::map<int,std::unique_ptr<ReadWatcher>> _readers;
    
    /**
     *  Set of writers
     *  @var    std::map
     */
    std::map<int,std::unique_ptr<WriteWatcher>> _writers;

    /**
     *  Timer
     *  @var    TimeoutWatcher
     */
    TimeoutWatcher _timer;
    
    /**
     *  Number of pending calls
     *  @var    integer
     */
    int _pending = 0;

protected:
    /**
     *  Pointer to the loop
     *  @var    Loop
     */
    Loop *_loop;
    
    /**
     *  The channel
     *  @var    ares_channel
     */
    ares_channel _channel = nullptr;
    
protected:
    /**
     *  Constructor
     *  @param  loop
     */
    Base(Loop *loop);
    
    /**
     *  Set the timeout for the next iteration
     */
    void setTimeout();
    
public:
    /**
     *  Destructor
     */
    virtual ~Base();
    
    /**
     *  Check a certain filedescriptor for readability or writability
     *  @param  fd      Filedescriptor
     *  @param  read    Check for readability
     *  @param  write   Check for writability
     */
    void check(int fd, bool read, bool write);
    
    /**
     *  Increment number of pending calls
     */
    void increment()
    {
        // increment pending calls
        _pending++;
    }
    
    /**
     *  Decrement number of pending calls
     */
    void decrement()
    {
        // decrement number of pending calls
        if (--_pending > 0) return;
        
        // nothing is pending, cancel the timer
        _timer.cancel();
    }
};

/**
 *  End namespace
 */
}}

