/**
 *  Base.cpp
 *
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace React { namespace Dns {

/**
 *  Callback method that is called when a socket changes state
 *  @param  data    Used-supplied-data (pointer to the resolver)
 *  @param  fd      The filedescriptor
 *  @param  read    Does the socket want to be notified when it is readable
 *  @param  write   Does the socket want to be notified when it is writable
 */
static void socket_state_callback(void *data, int fd, int read, int write)
{
    // retrieve a pointer to the resolver
    Base *resolver = (Base *)data;
    
    // change the state
    resolver->check(fd, read, write);
}

/**
 *  Callback method that is called when a socket is created
 *  @param  fd      The socket filedescriptor
 *  @param  type    Type of socket (???)
 *  @param  data    User-supplied-data (pointer to the resolver)
 */
static int socket_callback(ares_socket_t fd, int type, void *data)
{
    // we assume success (in theory ARES_ENOMEM could be returned, but when 
    // that happens the whole computer has a big problem)
    return ARES_SUCCESS;
}

/**
 *  Static function that is called on expiration of a timer
 *  @param  channel
 */
static void timer_expires(ares_channel channel)
{
    // sets of readable and writable filedescriptors
    fd_set readable;
    fd_set writable;
    
    // both sets are empty
    FD_ZERO(&readable);
    FD_ZERO(&writable);
    
    // process everything
    ares_process(channel, &readable, &writable);
}

/**
 *  Constructor
 *  @param  loop        Loop in which the resolver is activated
 */
Base::Base(Loop *loop) : _timer(loop, 0.0, [this]() { timer_expires(_channel); setTimeout(); }), _loop(loop)
{
    // initialisation options
    struct ares_options options;
    
    // set the callback function
    options.sock_state_cb = socket_state_callback;
    options.sock_state_cb_data = this;
    
    // initialize the channel
    if (ares_init_options(&_channel, &options, ARES_OPT_SOCK_STATE_CB) != ARES_SUCCESS) return;
        
    // set callback method for creating sockets
    ares_set_socket_callback(_channel, socket_callback, this);
}

/**
 *  Destructor
 */
Base::~Base()
{
    // leap out if channel is not set
    if (!_channel) return;
    
    // cancel all current requests
    ares_cancel(_channel);
    
    // destroy the channel
    ares_destroy(_channel);
}

/**
 *  Set the timeout for the next iteration
 */
void Base::setTimeout()
{
    // do we need a timer in the first place?
    if (_pending == 0)
    {
        // no calls are pending, stop the timer
        _timer.cancel();
    }
    else
    {
        // max timeout is 10 seconds
        Timeval timeval(10.0);
        
        // calculate new timestamp
        ares_timeout(_channel, &timeval, &timeval);
        
        // set the timeout
        _timer.set(timeval);
    }
}

/**
 *  Check a certain filedescriptor for readability or writability
 *  @param  fd      Filedescriptor
 *  @param  read    Check for readability
 *  @param  write   Check for writability
 */
void Base::check(int fd, bool read, bool write)
{
    // remove from the appriate sets
    if (!read) _readers.erase(fd);
    if (!write) _writers.erase(fd);
    
    // insert new entry for readability
    if (read && _readers.find(fd) == _readers.end())
    {
        // add a new reader
        auto *watcher = new ReadWatcher(_loop, fd, [this,fd]() -> bool { 
            ares_process_fd(_channel, fd, 0);
            return true;
        });
        
        // add to the set of watcher
        _readers[fd] = std::unique_ptr<ReadWatcher>(watcher);
    }
    
    // insert new entry for writability
    if (write && _writers.find(fd) == _writers.end()) 
    {
        // add a watcher
        auto *watcher = new WriteWatcher(_loop, fd, [this,fd]() -> bool { 
            ares_process_fd(_channel, 0, fd); 
            return true;
        });
        
        // add to the set of watchers
        _writers[fd] = std::unique_ptr<WriteWatcher>(watcher);
    }
}

/**
 *  End namespace
 */
}}

