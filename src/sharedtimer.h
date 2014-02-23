/**
 *  SharedTimer.h
 *
 *  Timer that is managed by the loop, and that can be shared with the outside
 *  world.
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
class SharedTimer : public Shared<Timer>, public Timer
{
private:
    /**
     *  Called when timer expires
     */
    virtual void invoke() override
    {
        // keep a shared pointer for as long as the callback is called
        auto ptr = pointer();
        
        // now we call the base invoke method
        Timer::invoke();
    }

public:
    /**
     *  Constructor
     *  @param  loop
     *  @param  timeout
     *  @param  callback
     */
    SharedTimer(Loop *loop, Timestamp timeout, const TimerCallback &callback) : Shared(this), Timer(loop, timeout, callback) {}
    
    /**
     *  Destructor
     */
    virtual ~SharedTimer() {}
    
    /**
     *  Start the timer
     *  @return bool
     */
    virtual bool start() override
    {
        // call base
        if (!Timer::start()) return false;
        
        // object lives again, fix the shared pointer
        restore();
        
        // done
        return true;
    }
    
    /**
     *  Cancel the timer
     *  @return bool
     */
    virtual bool cancel() override
    {
        // call base
        if (!Timer::cancel()) return false;
        
        // object is cancelled, forget shared pointer
        reset();
        
        // done
        return true;
    }
};

/**
 *  End namespace
 */
}
