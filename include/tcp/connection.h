/**
 *  Connection.h
 *
 *  Class that represents a TCP connection, or TCP connection attempt. It 
 *  can be constructed with either a Tcp::Server parameter (which should be
 *  in a readable state) to accept an incoming connection, or with the address
 *  of a remote peer for an outgoing connection.
 *
 *  If you use this for outgoing connections, you should first check for
 *  writability to wait for the connection to be ready. After it is writable
 *  you can check whether it is connected, and start using it for either
 *  sending or receiving.
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React { namespace Tcp {
    
/**
 *  Class definition
 */
class Connection
{
private:
    /**
     *  The underlying TCP socket
     *  @var    Socket
     */
    Socket _socket;
    
    /**
     *  Status of the connection
     *  @var    enum
     */
    enum {
        connecting,
        connected,
        closing,
        closed
    } _status = connecting;
    
public:
    /**
     *  Constructor
     *  @param  server      Tcp::Server object that is in readable state, and for which we'll accept the connection
     */
    Connection(Server *server) : _socket(server->_socket.accept()), _status(connected) 
    {
        // @todo set up handlers
    }

    /**
     *  Constructor
     *  @param  server      Tcp::Server object that is in readable state, and for which we'll accept the connection
     */
    Connection(const Server &server) : _socket(std::move(server._socket.accept())), _status(connected) 
    {
        // @todo set up handlers
    }
    
    /**
     *  Connection can not be copied
     *  @param  connection
     */
    Connection(const Connection &connection) = delete;
    
    /**
     *  Move constructor
     *  @param  connection
     */
    Connection(Connection &&connection) : _socket(std::move(connection._socket)) 
    {
        // @todo set up / move handlers
    }
    
    /**
     *  Destructor
     */
    virtual ~Connection() {}
    
    /**
     *  Attach a handler for readability
     * 
     *  This will overwrite an earlier handler that you had attached
     * 
     *  @param  callback
     *  @return ReadWatcher
     */
    template <typename CALLBACK>
    std::shared_ptr<ReadWatcher> onReadable(const CALLBACK &callback)
    {
        // pass on to the socket
        return _socket.onReadable(callback);
    }
    
    /**
     *  Attach a handler for writability
     *  
     *  This will overwrite an earlier handler that you had attached
     * 
     *  @param  callback
     *  @return WriteWatcher
     */
    template <typename CALLBACK>
    std::shared_ptr<WriteWatcher> onWritable(const CALLBACK &callback)
    {
        // pass on to the socket
        return _socket.onWritable(callback);
    }
    
    /**
     *  Send data to the connection
     * 
     *  This method is directly forwarded to the ::send() system call.
     * 
     *  @param  buf     Pointer to a buffer
     *  @param  len     Size of the buffer
     *  @param  flags   Optional additional flags
     *  @return ssize_t Number of bytes sent
     */
    ssize_t send(const void *buf, size_t len, int flags = 0) const
    {
        return _socket.send(buf, len, flags);
    }
    
    /**
     *  Receive data from the connection
     * 
     *  This method is directly forwarded to the ::recv() system call.
     * 
     *  @param  buf     Pointer to a buffer
     *  @param  len     Size of the buffer
     *  @param  flags   Optional additional flags
     *  @return ssize_t Number of bytes received
     */
    ssize_t recv(void *buf, size_t len, int flags = 0) const
    {
        return _socket.recv(buf, len, flags);
    }
};

/**
 *  End namespace
 */
}}

