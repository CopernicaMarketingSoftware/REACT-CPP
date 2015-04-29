/**
 *  process.h
 *
 *  Manage an external process started in
 *  the event loop
 *
 *  @copyright 2015 Copernica B.V.
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  Set up namespace
 */
namespace React {

/**
 *  Class definition
 */
class Process
{
private:
    /**
     *  The loop monitoring the process
     *  @var    MainLoop
     */
    MainLoop *_loop;

    /**
     *  Pipe for writing to the process
     *  @var    Pipe
     */
    Pipe _stdin;

    /**
     *  Pipe for reading standard output from the process
     *  @var    Pipe
     */
    Pipe _stdout;

    /**
     *  Pipe for reading standard errors from the process
     *  @var    Pipe
     */
    Pipe _stderr;

    /**
     *  Process id of the running process
     *  @var    pid_t
     */
    pid_t _pid;

    /**
     *  Listen to the child state changes
     *  @var    StatusWatcher
     */
    StatusWatcher _watcher;

    /**
     *  Is the program currently running?
     *  @var    bool
     */
    bool _running = false;

    /**
     *  Callback to invoke on status changes
     *  @var    std::function<void(int status)>
     */
    std::function<void(int status)> _callback;
public:
    /**
     *  Constructor
     *
     *  @param  loop        The loop that is responsible for the process
     *  @param  program     The program to execute
     *  @param  arguments   Null-terminated array of arguments
     */
    Process(MainLoop *loop, const char *program, const char *arguments[]);

    /**
     *  Destructor
     */
    virtual ~Process();

    /**
     *  Register a handler for status changes
     *
     *  Note that if you had already registered a handler before, then that one
     *  will be reset. Only your new handler will be called when the child process
     *  changes state.
     *
     *  The callback will be invoked with a single parameter named 'status'. For
     *  more information on the meaning of this parameter, see the manual for
     *  waitpid(2) or sys/wait.h
     *
     *  @param  callback    The callback to invoke when the process changes state
     */
    void onStatusChange(const std::function<void(int status)> &callback);

    /**
     *  Get the file descriptor linked to the stdin of
     *  the running program.
     *
     *  This file descriptor can be used to write data
     *  to the running program.
     *
     *  @return the stdin file descriptor
     */
    Fd &stdin();

    /**
     *  Register a handler for writability
     *
     *  Note that if you had already registered a handler before, then that one
     *  will be reset. Only your new handler will be called when the filedescriptor
     *  becomes writable
     *
     *  @param  callback    The callback to invoke when the process is ready to receive input
     *  @return WriteWatcher
     */
    std::shared_ptr<WriteWatcher> onWritable(const WriteCallback &callback);

    /**
     *  Get the file descriptor linked to the stdout
     *  of the running program.
     *
     *  This file descriptor can be used to read data
     *  that the running program outputs to the
     *  standard output.
     *
     *  @return the stdout file descriptor
     */
    Fd &stdout();

    /**
     *  Register a handler for readability
     *
     *  Note that if you had already registered a handler before, then that one
     *  will be reset. Only your new handler will be called when the filedescriptor
     *  becomes readable
     *
     *  @param  callback    The callback to invoke when the process has generated output
     *  @return ReadWatcher
     */
    std::shared_ptr<ReadWatcher> onReadable(const ReadCallback &callback);

    /**
     *  Get the file descriptor linked to the stderr
     *  of the running program.
     *
     *  This file descriptor can be used to read data
     *  that the running program outputs to the
     *  standard error.
     *
     *  @return the stderr file descriptor
     */
    Fd &stderr();

    /**
     *  Register a handler for errorability
     *
     *  Note that if you had already registered a handler before, then that one
     *  will be reset. Only your new handler will be called when the filedescriptor
     *  becomes readable
     *
     *  @param  callback    The callback to invoke when the process has generated errors
     *  @return ReadWatcher
     */
    std::shared_ptr<ReadWatcher> onError(const ReadCallback &callback);
};

/**
 *  End namespace
 */
}
