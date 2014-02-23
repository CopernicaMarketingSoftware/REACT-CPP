/**
 *  MainLoop.h
 * 
 *  The main loop is an extended loop class that can also be used to watch
 *  for signals. In normal circumstances you only need to create one
 *  main loop, in the master thread. If you start additional threads, you can
 *  use the regular loop class.
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
class MainLoop : public Loop
{
public:
    /**
     *  Constructor
     */
    MainLoop() : Loop(ev_default_loop(EVFLAG_AUTO)) {}
    
    /**
     *  Destructor
     */
    virtual ~MainLoop();
    
    /**
     *  Register a function that is called the moment a signal is fired.
     *
     *  This method takes two arguments: the signal number to be checked and
     *  the function that is going to be called the moment the signal is 
     *  triggered.
     *
     *  The method returns a shared pointer to a watcher object. This watcher
     *  object can be used to later stop watching for the signal. It is 
     *  legal to ignore the return value if you don't need it. In that case
     *  the signal is watched for the entire runtime of the loop.
     * 
     *  @param  signum      The signal
     *  @param  callback    Function that is called the moment the signal is caught
     *  @return             Object that can be used to stop checking for signals
     */
    std::shared_ptr<Signal> onSignal(int signum, const SignalCallback &callback);
    std::shared_ptr<Signal> onSignal(int signum, const Callback &callback) { return onSignal(signum, [callback](Signal *signal) { callback(); }); }
};
    
/**
 *  End of namespace
 */
}
