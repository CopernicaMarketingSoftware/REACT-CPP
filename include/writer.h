/**
 *  Writer.h
 *
 *  Class to watch a filedescriptor for readability
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
class Writer : private Watcher
{
private:
    /**
     *  Pointer to the loop
     *  @var    Loop
     */
    Loop *_loop;

    /**
     *  IO resource
     *  @var    ev_io
     */
    struct ev_io _watcher;
    
    /**
     *  Callback function
     *  @var    WriteCallback
     */
    WriteCallback _callback;
    
    /**
     *  Is it active?
     *  @var    bool
     */
    bool _active = false;

    /**
     *  Function to initialize the io watcher
     *  @param  fd
     */
    void initialize(int fd);

protected:
    /**
     *  Call the reader (which in turn will call the handler)
     */
    virtual void invoke() override
    {
        // call the callback
        _callback(this);
    }

public:
    /**
     *  Constructor
     *  @param  loop        The event loop
     *  @param  fd          File descriptor
     *  @param  callback    Function called when filedescriptor becomes readable
     */
    Writer(Loop *loop, int fd, const WriteCallback &callback) : 
        _loop(loop), _callback(callback)
    {
        // store pointer to current object
        _watcher.data = this;
        
        // initialize the watcher
        initialize(fd);
        
        // start (resume) the watcher
        resume();
    }

    /**
     *  No copying or moving allowed
     *  @param  that
     */
    Writer(const Writer &that) = delete;
    Writer(Writer &&that) = delete;

    /**
     *  Destructor
     */
    virtual ~Writer()
    {
        cancel();
    }

    /**
     *  No copying or moving
     *  @param  that
     */
    Timer &operator=(const Timer &that) = delete;
    Timer &operator=(Timer &&that) = delete;
    
    /**
     *  Cancel the watcher
     *  @return bool
     */
    virtual bool cancel()
    {
        // must be active
        if (!_active) return false;
        
        // stop watcher
        ev_io_stop(*_loop, &_watcher);
        
        // no longer active
        _active = false;
        
        // done
        return true;
    }
    
    /**
     *  Start/resume the watcher
     *  @return bool
     */
    virtual bool resume()
    {
        // should not be active
        if (_active) return false;
        
        // start it
        ev_io_start(*_loop, &_watcher);
        
        // done
        return _active = true;
    }
};

/**
 *  End namespace
 */
}

