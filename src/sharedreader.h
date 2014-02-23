/**
 *  SharedReader.h
 *
 *  Class that is only used internally, and that is constructed for readers
 *  that are constructed via the Loop::onReadable() method. It holds a shared
 *  pointer to itself
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
class SharedReader : public Shared<Reader>, public Reader
{
private:
    /**
     *  Called when filedescriptor is readable
     */
    virtual void invoke() override
    {
        // keep a shared pointer for as long as the callback is called, this
        // ensures that the object is not destructed if the user calls 
        // the cancel() method, and immediately afterwards another method
        auto ptr = pointer();
        
        // now we call the base invoke method
        Reader::invoke();
    }


public:
    /**
     *  Constructor
     *  @param  loop        The event loop
     *  @param  fd          File descriptor
     *  @param  callback    Function called when filedescriptor becomes readable
     */
    SharedReader(Loop *loop, int fd, const ReadCallback &callback) : Shared(this), Reader(loop, fd, callback) {}
    
    /**
     *  Destructor
     */
    virtual ~SharedReader() {}

    /**
     *  Start/resume the watcher
     *  @return bool
     */
    virtual bool resume() override
    {
        // call base
        if (!Reader::resume()) return false;
        
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
        if (!Reader::cancel()) return false;
        
        // because the watcher is no longer running, we no longer have to keep a pointer to ourselves
        reset();
        
        // done
        return true;
    }
};

/**
 *  End namespace
 */
}

