/**
 *  pipe.cpp
 *
 *  A class representing a non-blocking pipe, used
 *  for communicating between different processes.
 *
 *  @copyright 2015 Copernica B.V.
 */
#include "includes.h"
#include <fcntl.h>
#include <unistd.h>

/**
 *  Start namespace
 */
namespace React {

/**
 *  Initialize a pipe at the given address
 *
 *  @param  pipefd  The file descriptors
 *  @return The first file descriptor
 */
static int initialize(std::array<int, 2> &pipefd)
{
    // initialize the pipes
    if (pipe2(pipefd.data(), O_NONBLOCK /* | O_CLOEXEC */)) throw std::runtime_error(strerror(errno));

    // return the first pipe (used by the main process)
    return pipefd[0];
}

/**
 *  Constructor
 *
 *  @param  loop    the loop to bind to
 */
Pipe::Pipe(Loop *loop) :
    _read(loop, initialize(_fds)),
    _write(loop, _fds[1])
{}

/**
 *  Destructor
 */
Pipe::~Pipe()
{
    // clean up the pipes
    for (auto fd : _fds)
    {
        // check if it is a valid file descriptor and close it
        if (fd > 0) close(fd);
    }
}

/**
 *  Close the file descriptor at the given offset
 *
 *  @param  offset  The offset in the array to close
 */
void Pipe::close(size_t offset)
{
    // not necessary if it is already closed
    if (_fds[offset] == 0) return;

    // close the file descriptor
    ::close(_fds[offset]);

    // set it to zero to prevent multiple closures
    _fds[offset] = 0;
}

/**
 *  Retrieve the file descriptor for reading
 *
 *  @return The file descriptor
 */
Fd &Pipe::read()
{
    // close the write fd
    close(1);

    // return member
    return _read;
}

/**
 *  Retrieve the file descriptor for writing
 *
 *  @return The file descriptor
 */
Fd &Pipe::write()
{
    // close the read fd
    close(0);

    // return member
    return _write;
}

/**
 *  Register a handler for readability
 *
 *  Note that if you had already registered a handler before, then that one
 *  will be reset. Only your new handler will be called when the filedescriptor
 *  becomes readable
 *
 *  @param  callback    The callback to invoke when data is available to be read
 *  @return Watcher object to cancel reading
 */
std::shared_ptr<ReadWatcher> Pipe::onReadable(const ReadCallback &callback)
{
    // close the write fd
    close(1);

    // register read watcher
    return _read.onReadable(callback);
}

/**
 *  Register a handler for writability
 *
 *  Note that if you had already registered a handler before, then that one
 *  will be reset. Only your new handler will be called when the filedescriptor
 *  becomes writable
 *
 *  @param  callback    The callback to invoke when data can be sent
 *  @return Watcher object to cancel writing
 */
std::shared_ptr<WriteWatcher> Pipe::onWritable(const WriteCallback &callback)
{
    // close the read fd
    close(0);

    // register write watcher
    return _write.onWritable(callback);
}

/**
 *  End namespace
 */
}
