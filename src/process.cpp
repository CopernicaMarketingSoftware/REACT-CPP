/**
 *  process.cpp
 *
 *  Manage an external process started in
 *  the event loop
 *
 *  @copyright 2015 Copernica B.V.
 */
#include "includes.h"
#include <unistd.h>
#include <sys/wait.h>

/**
 *  Set up namespace
 */
namespace React {

/**
 *  Constructor
 *
 *  @param  loop        The loop that is responsible for the process
 *  @param  program     The program to execute
 *  @param  arguments   Null-terminated array of arguments
 */
Process::Process(MainLoop *loop, const std::function<void()> &callback, const char *program, const char *arguments[]) :
    _loop(loop),
    _callback(callback),
    _watcher(loop, SIGCHLD, std::bind(&Process::onSignal, this))
{
    // we need to keep the loop running until the program has finished
    ev_ref(*_loop);

    // initialize the pipes
    if (pipe(_stdin))   throw std::runtime_error(strerror(errno));
    if (pipe(_stdout))  throw std::runtime_error(strerror(errno));
    if (pipe(_stderr))  throw std::runtime_error(strerror(errno));

    // fork before executing the process
    _pid = fork();

    // did the fork succeed?
    if (_pid < 0)
    {
        // oops, forking failed
        throw std::runtime_error(strerror(errno));
    }
    // are we running in the main process
    else if (_pid)
    {
        // close the child part of the pipes
        close(_stdin[1]);
        close(_stdout[1]);
        close(_stderr[1]);
    }
    // or the child process
    else
    {
        // close the master part of the pipes
        close(_stdin[0]);
        close(_stdout[0]);
        close(_stderr[0]);

        // use the provided pipes for stdin, stdout and stderr
        dup2(_stdin[1], STDIN_FILENO);
        dup2(_stdout[1], STDOUT_FILENO);
        dup2(_stderr[1], STDERR_FILENO);

        // execute the requested program
        execvp(program, const_cast<char **>(arguments));
    }
}

/**
 *  Destructor
 */
Process::~Process()
{
    // close our side of the pipes
    close(_stdin[0]);
    close(_stdout[0]);
    close(_stderr[0]);
}

/**
 *  Handle SIGCHLD signals
 *
 *  @return Do we continue watching for signals?
 */
bool Process::onSignal()
{
    // waitpid returns non-zero when our child has now exited
    // which is exactly when we want to stop monitoring
    if (!waitpid(_pid, nullptr, WNOHANG)) return true;

    // the program has finished, the loop may now stop
    // if we were the only ones keeping it alive
    ev_unref(*_loop);

    // alright, the process has finished running
    _callback();

    // we may now stop monitoring for signals
    return false;
}

/**
 *  Get the file descriptor linked to the stdin of
 *  the running program.
 *
 *  This file descriptor can be used to write data
 *  to the running program.
 *
 *  @return the stdin file descriptor
 */
int Process::stdin()
{
    // return the master side of the stdin pipe
    return _stdin[0];
}

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
int Process::stdout()
{
    // return the master side of the stdin pipe
    return _stdout[0];
}

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
int Process::stderr()
{
    // return the master side of the stdin pipe
    return _stderr[0];
}

/**
 *  End namespace
 */
}
