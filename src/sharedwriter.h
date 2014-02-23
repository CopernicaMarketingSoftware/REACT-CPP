/**
 *  SharedWriter.h
 *
 *  Class that is only used internally, and that is constructed for writers
 *  that are constructed via the Loop::onWritable() method. It holds a shared
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
class SharedWriter : public Shared<Writer>, public Writer
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
        Writer::invoke();
    }

public:
    /**
     *  Constructor
     *  @param  loop        The event loop
     *  @param  fd          File descriptor
     *  @param  callback    Function called when filedescriptor becomes writable
     */
    SharedWriter(Loop *loop, int fd, const WriteCallback &callback) : Shared(this), Writer(loop, fd, callback) {}
    
    /**
     *  Destructor
     */
    virtual ~SharedWriter() {}

    /**
     *  Start/resume the watcher
     *  @return bool
     */
    virtual bool resume() override
    {
        // call base
        if (!Writer::resume()) return false;
        
        // make sure the shared pointer is valid, so that we have a reference to ourselves
        restore();
        
        // done
        return true;
    }
    
    /**
     *  Cancel the watcher
     *  @return bool
     */
    virtual bool cancel() override
    {
        // call base
        if (!Writer::cancel()) return false;
        
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

