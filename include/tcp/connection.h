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
        closed
    } _status = connecting;
    
    /**
     *  The handler for readability
     *  @var    ReadCallbacl
     */
    ReadCallback _readCallback;
    
    /**
     *  The handler for writability
     *  @var    WriteCallback
     */
    WriteCallback _writeCallback;
    
    /**
     *  The handler when the connection closes
     *  @var    LostCallback
     */
    LostCallback _lostCallback;
    
    /**
     *  Reset the object
     */
    void reset()
    {
        // change status
        _status = closed;
        
        // remove all callbacks, because they may have captured pointers that
        // should be destructed
        _readCallback = ReadCallback();
        _writeCallback = WriteCallback();
        _lostCallback = LostCallback();
    }
    
public:
    /**
     *  Constructor
     *  @param  server      Tcp::Server object that is in readable state, and for which we'll accept the connection
     */
    Connection(const Server *server) : _socket(std::move(server->_socket.accept())), _status(connected) {}

    /**
     *  Constructor
     *  @param  server      Tcp::Server object that is in readable state, and for which we'll accept the connection
     */
    Connection(const Server &server) : Connection(&server) {}
    
    /**
     *  Constructor to connect to a socket
     *  @param  loop        Event loop
     *  @param  fromip      IP address to connect from
     *  @param  fromport    Port number to connect from
     *  @param  toip        IP address to connect to
     *  @param  toport      Port number to connect to
     *  @param  callback    Callback that is notified when ready
     */
    Connection(Loop *loop, const Net::Ip &fromip, uint16_t fromport, const Net::Ip &toip, uint16_t toport, const ConstructCallback &callback) :
        _socket(loop, fromip, fromport), _status(connecting)
    {
        // try connecting
        if (!_socket.connect(toip, toport)) throw Exception(strerror(errno));
        
        // install the onConnected callback
        onConnected(std::bind(callback, this, std::placeholders::_1));
    }

    /**
     *  Constructor to connect to a socket
     *  @param  loop        Event loop
     *  @param  fromip      IP address to connect from
     *  @param  toip        IP address to connect to
     *  @param  toport      Port number to connect to
     *  @param  callback    Callback that is notified when ready
     */
    Connection(Loop *loop, const Net::Ip &fromip, const Net::Ip &toip, uint16_t toport, const ConstructCallback &callback) :
        Connection(loop, fromip, 0, toip, toport, callback) {}

    /**
     *  Constructor to connect to a socket
     *  @param  loop        Event loop
     *  @param  toip        IP address to connect to
     *  @param  toport      Port number to connect to
     *  @param  callback    Callback that is notified when ready
     */
    Connection(Loop *loop, const Net::Ip &toip, uint16_t toport, const ConstructCallback &callback) :
        Connection(loop, toip.version() == 6 ? Net::Ip(Net::Ipv6()) : Net::Ip(Net::Ipv4()), 0, toip, toport, callback) {}

    /**
     *  Constructor to connect to a socket
     *  @param  loop        Event loop
     *  @param  from        From address
     *  @param  to          To address
     *  @param  callback    Callback that is notified when ready
     */
    Connection(Loop *loop, const Net::Address &from, const Net::Address &to, const ConstructCallback &callback) :
        Connection(loop, from.ip(), from.port(), to.ip(), to.port(), callback) {}

    /**
     *  Constructor to connect to a socket
     *  @param  loop        Event loop
     *  @param  to          To address
     *  @param  callback    Callback that is notified when ready
     */
    Connection(Loop *loop, const Net::Address &to, const ConstructCallback &callback) :
        Connection(loop, to.ip().version() == 6 ? Net::Ip(Net::Ipv6()) : Net::Ip(Net::Ipv4()), 0, to.ip(), to.port(), callback) {}
    
    /**
     *  Connection can not be copied
     *  @param  connection
     */
    Connection(const Connection &connection) = delete;
    
    /**
     *  Move constructor
     *  @param  connection
     */
    Connection(Connection &&connection) = delete;
    
    /**
     *  Destructor
     */
    virtual ~Connection() {}
    
    /**
     *  Install a handler that is called when the socket is connected
     *  This is only meaningful if the socket is busy connecting (right after it was constructed)
     *  @param  callback
     */
    void onConnected(const ConnectedCallback &callback)
    {
        // must be busy connecting
        if (_status != connecting) return;
        
        // wait until writable
        _socket.onWritable([this, callback]() {
            
            // is the socket connected?
            if (_socket.connected())
            {
                // change status
                _status = connected;
                
                // install the callbacks if the user had already assigned them
                if (_readCallback) _socket.onReadable(_readCallback);
                if (_writeCallback) _socket.onWritable(_writeCallback);
                
                // we no longer need the cached callbacks
                _readCallback = nullptr;
                _writeCallback = nullptr;
                
                // report to callback
                callback(nullptr);
            }
            else
            {
                // reset the object
                reset();

                // @todo can we get a strerror?
                callback("Connect failure");
            }
            
            // no other writability events please
            return false;
        });
    }
    
    /**
     *  Check for readability
     * 
     *  This method is called every time data is available to be read. If you had
     *  installed a different readability handler before, this second call will
     *  overwrite the handler that you installed before.
     * 
     *  @param  callback
     */
    void onReadable(const ReadCallback &callback)
    {
        // skip if already closed
        if (_status == closed) return;

        // start right away if the socket is already connected
        if (_status == connected) _socket.onReadable(_readCallback);
        
        // else store the callback for when the connection is ready
        else _readCallback = callback;
    }
    
    /**
     *  Check for writability
     *  
     *  This method is called every time the socket is writable and output 
     *  can be sent to it. If you had installed a different handler before,
     *  it will be overwritten by this handler
     * 
     *  @param  callback
     */
    void onWritable(const WriteCallback &callback)
    {
        // skip if already closed
        if (_status == closed) return;

        // start right away
        if (_status == connected) _socket.onWritable(callback);

        // else store the callback for when the connection is ready
        else _writeCallback = callback;
    }
    
    /**
     *  Check if the connection is lost
     * 
     *  The registered method will be called when the connection is lost
     * 
     *  @param  callback
     */
    void onLost(const LostCallback &callback)
    {
        // skip if already closed
        if (_status == closed) return;

        // install the callback
        _lostCallback = callback;
    }
    
    /**
     *  Check for data to come in
     * 
     *  This method is called for all data that comes in via the connection.
     *  If you set a data hander, the onReadable handler that you've set before
     *  will be overridden.
     * 
     *  @param  callback
     */
    void onData(const DataCallback &callback)
    {
        // install a readability handler
        onReadable([this, callback]() -> bool {
            
            // construct a pretty big buffer
            char buffer[4096];
            
            // receive the data
            ssize_t bytes = _socket.recv(buffer, 4096);
            
            // check for error
            if (bytes == 0 || (bytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK))
            {
                // remember the lost callback
                auto lostCallback = _lostCallback;
                
                // reset the object
                reset();
                
                // report error
                if (lostCallback) lostCallback();
                
                // we know enough
                return false;
            }
            else
            {
                // should be zero or more
                if (bytes <= 0) return true;
                
                // report
                return callback(buffer, bytes);
            }
        });
    }
    
    /**
     *  Send data to the connection
     * 
     *  This method is an almost direct call to the underlying ::send() system 
     *  call, and supports the same parameters. This means that you should be
     *  prepared to handle errors, and that not all bytes will always be sent.
     *  If not all bytes were sent, you should also install an onWritable handler
     *  to wait for the connection to become writable again, and try to send
     *  the remaining data then.
     * 
     *  If you need a smarter send() method that always works, you can wrap
     *  the connection class in an React::Tcp::Out() object, and send your
     *  data to that object instead.
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
     *  Send data to the connection
     * 
     *  This method is directly forwarded to the ::writev() system call. This 
     *  means that you should be prepared to handle errors, and that not all 
     *  bytes will always be sent. If not all bytes were sent, you should also 
     *  install an onWritable handler to wait for the connection to become 
     *  writable again, and try to send the remaining data then.
     * 
     *  If you need a smarter send() method that always works, you can wrap
     *  the connection class in an React::Tcp::Out() object, and send your
     *  data to that object instead.
     * 
     *  @param  iov     Array of struct iovec objects
     *  @param  iovcnt  Number of items in the array
     *  @return ssize_t Number of bytes sent
     */
    ssize_t writev(const struct iovec *iov, int iovcnt) const
    {
        return _socket.writev(iov, iovcnt);
    }
    
    /**
     *  Receive data from the connection
     * 
     *  This method is directly forwarded to the ::recv() system call. Note that
     *  if you have installed a onData() handler, you do not need this method
     *  and all incoming data is automatically passed on to your handler.
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
    
    /**
     *  Close the socket
     *  @return bool
     */
    bool close()
    {
        // close the socket
        if (!_socket.close()) return false;
        
        // object is invalid now, reset it
        reset();
        
        // done
        return true;
    }
};

/**
 *  End namespace
 */
}}

