/**
 *  Socket.h
 *
 *  TCP socket implementation
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
class Socket
{
private:
    /**
     *  Pointer to the event loop
     *  @var    Loop
     */
    Loop *_loop;

    /**
     *  Filedescriptor
     *  @var    int
     */
    int _fd;
    
    /**
     *  Helper method to bind the socket to an IPv4 address and port
     *  @param  ip          IP to bind to
     *  @param  port        Port to bind to
     *  @return bool
     */
    bool bind(const Net::Ipv4 &ip, uint16_t port)
    {
        // structure to initialize
        struct sockaddr_in info;
        
        // fill the members
        info.sin_family = AF_INET;
        info.sin_port = htons(port);
        
        // copy address
        memcpy(&info.sin_addr, ip.internal(), sizeof(struct in_addr));
        
        // bind
        return ::bind(_fd, (struct sockaddr *)&info, sizeof(struct sockaddr_in)) == 0;
    }

    /**
     *  Helper method to bind the socket to an IPv6 address and port
     *  @param  ip          IP to bind to
     *  @param  port        Port to bind to
     *  @return bool
     */
    bool bind(const Net::Ipv6 &ip, uint16_t port)
    {
        // structure to initialize
        struct sockaddr_in6 info;
        
        // fill the members
        info.sin6_family = AF_INET6;
        info.sin6_port = htons(port);
        info.sin6_flowinfo = 0;
        info.sin6_scope_id = 0;
        
        // copy the address
        memcpy(&info.sin6_addr, ip.internal(), sizeof(struct in6_addr));
        
        // bind
        return ::bind(_fd, (struct sockaddr *)&info, sizeof(struct sockaddr_in6)) == 0;
    }
    
    /**
     *  Bind the socket to an IP and port
     *  @param  ip          IP to bind to
     *  @param  port        Port to bind to
     *  @return bool
     */
    bool bind(const Net::Ip &ip, uint16_t port)
    {
        switch (ip.version()) {
        case 4: return bind(ip.v4(), port);
        case 6: return bind(ip.v6(), port);
        default:return false;
        }
    }
    
public:
    /**
     *  Constructor to directly bind the socket to an IP
     *  @param  loop        Event loop
     *  @param  ip          IP address to bind to
     *  @param  port        Port number to bind to (or 0 to use a random port)
     */
    Socket(Loop *loop, const Net::Ip &ip = Net::Ip(), uint16_t port = 0) : 
        _loop(loop), _fd(socket(ip.version() == 6 ? AF_INET6 : AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0))
    {
        // this should succeed
        if (_fd < 0) throw Exception(strerror(errno));
        
        // we are going to bind the socket
        if (!bind(ip, port)) throw Exception(strerror(errno));

        // mark the socket as a "keepalive" socket, to send a sort of ping
        // every once in a while to ensure that the other end is connected,
        // even when no traffic was seen on the socket for a long time
        int keepalive = 1;
        setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(int));
    }
    
    /**
     *  Constructor to bind the socket to a port
     *  @param  loop        Event loop
     *  @param  port        Port number to bind to (or 0 to use a random port)
     */
    Socket(Loop *loop, uint16_t port) : Socket(loop, Net::Ip(), port) {}
    
    /**
     *  Destructor
     */
    virtual ~Socket()
    {
        close(_fd);
    }
    
    /**
     *  Return the internal identifier
     *  @return int
     */
    int internal() const
    {
        return _fd;
    }
    
    /**
     *  Listen to a socket
     *  @param  backlog
     *  @return bool
     */
    bool listen(int backlog = 1)
    {
        return ::listen(_fd, backlog) == 0;
    }
    
    /**
     *  Check for readability
     *  @param  callback
     *  @return ReadWatcher
     */
    template <typename CALLBACK>
    std::shared_ptr<ReadWatcher> onReadable(const CALLBACK &callback)
    {
        // pass on to the loop
        // @todo    we should remember the reader so it can be cancelled when the socket is closed
        return _loop->onReadable(_fd, callback);
    }
    
    /**
     *  Check for writability
     *  @param  callback
     *  @return WriteWatcher
     */
    template <typename CALLBACK>
    std::shared_ptr<WriteWatcher> onWritable(const CALLBACK &callback)
    {
        // pass on to the loop
        // @todo    we should remember the reader so it can be cancelled when the socket is closed
        return _loop->onWritable(_fd, callback);
    }
};

/**
 *  End namespace
 */
}}

