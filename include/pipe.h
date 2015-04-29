/**
 *  pipe.h
 *
 *  A class representing a non-blocking pipe, used
 *  for communicating between different processes.
 *
 *  @copyright 2015 Copernica B.V.
 */

/**
 *  Set up namespace
 */
namespace React {

/**
 *  Class definition
 */
class Pipe
{
private:
    /**
     *  The file descriptors used in the pipe
     *  @var    std::array<int, 2>
     */
    std::array<int, 2> _fds;

    /**
     *  File descriptor for reading
     *  @var    Fd
     */
    Fd _read;

    /**
     *  File descriptor for writing
     *  @var    Fd
     */
    Fd _write;

    /**
     *  Close the file descriptor at the given offset
     *
     *  @param  offset  The offset in the array to close
     */
    void close(size_t offset);
public:
    /**
     *  Constructor
     *
     *  @param  loop    the loop to bind to
     */
    Pipe(Loop *loop);

    /**
     *  Destructor
     */
    virtual ~Pipe();

    /**
     *  Retrieve the file descriptor for reading
     *
     *  @return The file descriptor
     */
    Fd &read();

    /**
     *  Retrieve the file descriptor for writing
     *
     *  @return The file descriptor
     */
    Fd &write();

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
    std::shared_ptr<ReadWatcher> onReadable(const ReadCallback &callback);

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
    std::shared_ptr<WriteWatcher> onWritable(const WriteCallback &callback);
};

/**
 *  End namespace
 */
}
