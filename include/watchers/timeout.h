/**
 *  Timeout.h
 *
 *  Timer that fires once
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
class TimeoutWatcher : private Watcher
{
private:
    /**
     *  Pointer to the loop
     *  @var    Loop
     */
    Loop *_loop;

    /**
     *  IO resource
     *  @var    ev_timer
     */
    struct ev_timer _watcher;

    /**
     *  Callback function
     *  @var    TimeoutCallback
     */
    TimeoutCallback _callback;

    /**
     *  Is the timer active?
     *  @var    bool
     */
    bool _active = false;

    /**
     *  When should it expired?
     *  @var    Timestamp
     */
    Timestamp _expire;

    /**
     *  Initialize the object
     *  @param  timeout time until invocation
     */
    void initialize(Timestamp timeout);

protected:
    /**
     *  Invoke the callback
     */
    virtual void invoke() override
    {
        // is this indeed the expiration time?
        if (_expire <= _loop->now())
        {
            // timer is no longer active
            cancel();

            // notify parent (return value is not important, a timer is always
            // cancelled after it expired)
            _callback();
        }
        else
        {
            // we set a new timer
            set(_expire - _loop->now());
        }
    }

public:
    /**
     *  Constructor
     *  @param  loop        Event loop
     *  @param  timeout     Timeout period
     *  @param  callback    Function that is called when timer is expired
     */
    template <typename CALLBACK>
    TimeoutWatcher(Loop *loop, Timestamp timeout, const CALLBACK &callback) : _loop(loop), _callback(callback)
    {
        // store pointer to current object
        _watcher.data = this;

        // initialize the watcher
        initialize(timeout);

        // start the timer
        start();
    }

    /**
     *  Constructor
     *
     *  This constructor is used to create a timeout that is not
     *  initially started. After construction, the set() member
     *  function can be used to set a timeout and start
     */
    TimeoutWatcher(Loop *loop, const TimeoutCallback &callback) :
        _loop(loop), _callback(callback)
    {
        // store pointer to current object
        _watcher.data = this;

        // initialize the watcher
        initialize(0.0);
    }

    /**
     *  No copying or moving allowed
     *  @param  that
     */
    TimeoutWatcher(const TimeoutWatcher &that) = delete;
    TimeoutWatcher(TimeoutWatcher &&that) = delete;

    /**
     *  Destructor
     */
    virtual ~TimeoutWatcher()
    {
        // cancel the timer
        cancel();
    }

    /**
     *  No copying or moving
     *  @param  that
     */
    TimeoutWatcher &operator=(const TimeoutWatcher &that) = delete;
    TimeoutWatcher &operator=(TimeoutWatcher &&that) = delete;

    /**
     *  Start the timer
     *  @return bool
     */
    virtual bool start()
    {
        // skip if already running
        if (_active) return false;

        // start now
        ev_timer_start(*_loop, &_watcher);

        // remember that it is active
        return _active = true;
    }

    /**
     *  Cancel the timer
     *  @return bool
     */
    virtual bool cancel()
    {
        // skip if not running
        if (!_active) return false;

        // stop now
        ev_timer_stop(*_loop, &_watcher);

        // remember that it no longer is active
        _active = false;

        // done
        return true;
    }

    /**
     *  Set the timer to a new time
     *  @param  timeout new time to invocation
     *  @return bool
     */
    bool set(Timestamp timeout)
    {
        // is the timer still active, and is the new timeout after the current timeout?
        // in that case we won't reset the timer to not interfere with the event loop,
        // but only update the expire time, when the timer then expires, the callback
        // will not be called, but a new timer is going to be set instead
        if (_active && _loop->now() + timeout < _expire)
        {
            // set the new expire time
            _expire = _loop->now() + timeout;

            // done
            return true;
        }
        else
        {
            // cancel the current time
            cancel();

            // set a new timer
            ev_timer_set(&_watcher, timeout, 0.0);

            // remember expire time
            _expire = _loop->now() + timeout;

            // start the timer
            return start();
        }
    }
};

/**
 *  End namespace
 */
}
