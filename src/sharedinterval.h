/**
 *  SharedInterval.h
 *
 *  Interval that is managed by the loop, and that can be shared with the outside
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
class SharedInterval : public Shared<Interval>, public Interval
{
private:
    /**
     *  Called when timer expires
     */
    virtual void invoke() override
    {
        // keep a shared pointer for as long as the callback is called, this
        // ensures that the object is not destructed if the user calls 
        // the cancel() method, and immediately afterwards a different method
        auto ptr = pointer();
        
        // now we call the base invoke method
        Interval::invoke();
    }

public:
    /**
     *  Constructor
     *  @param  loop
     *  @param  initial
     *  @param  interval
     *  @param  callback
     */
    SharedInterval(Loop *loop, Timestamp initial, Timestamp interval, const IntervalCallback &callback) : Shared(this), Interval(loop, initial, interval, callback) {}
    
    /**
     *  Destructor
     */
    virtual ~SharedInterval() {}
    
    /**
     *  Start the timer
     *  @return bool
     */
    virtual bool start() override
    {
        // call base
        if (!Interval::start()) return false;
        
        // make sure the shared pointer is valid, so that we have a reference to ourselves
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
        if (!Interval::cancel()) return false;
        
        // because the interval is no longer running, we no longer have to keep a pointer to ourselves
        reset();
        
        // done
        return true;
    }
};

/**
 *  End namespace
 */
}
