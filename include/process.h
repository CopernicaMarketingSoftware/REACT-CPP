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
     *  Callback to invoke when the process finished
     *  @var    std::function<void()>
     */
    std::function<void()> _callback;

    /**
     *  Listen to the SIGCHLD signal
     *  indicating the process is done
     *  @var    SignalWatcher
     */
    SignalWatcher _watcher;

    /**
     *  Pipe for writing to the process
     *  @var    int[2]
     */
    int _stdin[2];

    /**
     *  Pipe for reading standard output from the process
     *  @var    int[2]
     */
    int _stdout[2];

    /**
     *  Pipe for reading standard errors from the process
     *  @var    int[2]
     */
    int _stderr[2];

    /**
     *  Process id of the running process
     *  @var    pid_t
     */
    pid_t _pid;

    /**
     *  Handle SIGCHLD signals
     *
     *  @return Do we continue watching for signals?
     */
    bool onSignal();
public:
    /**
     *  Constructor
     *
     *  @param  loop        The loop that is responsible for the process
     *  @param  callback    The callback that is invoked when the process terminates
     *  @param  program     The program to execute
     *  @param  arguments   Null-terminated array of arguments
     */
    Process(MainLoop *loop, const std::function<void()> &callback, const char *program, const char *arguments[]);

    /**
     *  Destructor
     */
    virtual ~Process();

    /**
     *  Get the file descriptor linked to the stdin of
     *  the running program.
     *
     *  This file descriptor can be used to write data
     *  to the running program.
     *
     *  @return the stdin file descriptor
     */
    int stdin();

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
    int stdout();

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
    int stderr();
};

/**
 *  End namespace
 */
}
