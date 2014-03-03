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
class Socket : public Fd
{
private:
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

    /**
     *  Constructor to create a socket object by wrapping it around an existing socket
     *  @param  loop        Event loop
     *  @param  fd          The socket filedescriptor
     */
    Socket(Loop *loop, int fd) : Fd(loop, fd) 
    {
        // must be valid
        if (_fd < 0) throw Exception(strerror(errno));
        
        // mark the socket as a "keepalive" socket, to send a sort of ping
        // every once in a while to ensure that the other end is connected,
        // even when no traffic was seen on the socket for a long time
        int keepalive = 1;
        setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(int));
    }
    
public:
    /**
     *  Constructor to directly bind the socket to an IP and port
     * 
     *  Watch out! The constructor will throw an exception in case of an error.
     * 
     *  @param  loop        Event loop
     *  @param  ip          IP address to bind to
     *  @param  port        Port number to bind to (or 0 to use a random port)
     */
    Socket(Loop *loop, const Net::Ip &ip = Net::Ip(), uint16_t port = 0) : 
        Fd(loop, socket(ip.version() == 6 ? AF_INET6 : AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0))
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
     *  Sockets can not be copied
     *  @param  socket
     */
    Socket(const Socket &socket) = delete;
    
    /**
     *  Move constructor
     *  @param  socket
     */
    Socket(Socket &&socket) : Fd(std::move(socket)) {}
    
    /**
     *  Destructor
     */
    virtual ~Socket()
    {
        // close the filedescriptor
        close();
    }
    
    /**
     *  Is the socket connected?
     *  @return bool
     */
    bool connected() const
    {
        // filedescriptor must be valid
        if (_fd < 0) return false;
        
        // check peer address
        return PeerAddress(_fd).valid();
    }

    /**
     *  Connect the socket to a remote IPv4 address
     *  @param  ip
     *  @param  port
     *  @return bool
     */
    bool connect(const Net::Ipv4 &ip, uint16_t port) const
    {
        // structure to initialize
        struct sockaddr_in info;
        
        // fill the members
        info.sin_family = AF_INET;
        info.sin_port = htons(port);
        
        // copy address
        memcpy(&info.sin_addr, ip.internal(), sizeof(struct in_addr));
        
        // connect
        return ::connect(_fd, (struct sockaddr *)&info, sizeof(struct sockaddr_in)) == 0 || errno == EINPROGRESS;
    }

    /**
     *  Connect the socket to a remote IPv6 address
     *  @param  ip
     *  @param  port
     *  @return bool
     */
    bool connect(const Net::Ipv6 &ip, uint16_t port) const
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
        
        // connect
        return ::connect(_fd, (struct sockaddr *)&info, sizeof(struct sockaddr_in6)) == 0 || errno == EINPROGRESS;
    }
    
    /**
     *  Connect the socket to a remote IP
     *  @param  ip
     *  @param  port
     *  @return bool
     */
    bool connect(const Net::Ip &ip, uint16_t port) const
    {
        switch (ip.version()) {
        case 4: return connect(ip.v4(), port);
        case 6: return connect(ip.v6(), port);
        default:return false;
        }
    }

    /**
     *  Connect the socket to a remote address
     *  @param  address
     *  @return bool
     */
    bool connect(const Net::Address &address) const
    {
        return connect(address.ip(), address.port());
    }
    
    /**
     *  Listen to a socket
     *  @param  backlog
     *  @return bool
     */
    bool listen(int backlog = 1) const
    {
        // check for success
        if (::listen(_fd, backlog) != 0) return false;

        // set SO_REUSADDR for listening sockets
        int reuse = 1;
        setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
        
        // done
        return true;
    }
    
    /**
     *  Accept a connection on the socket
     * 
     *  Watch out! This method will throw an exception in case of an error.
     *
     *  @return Socket
     */
    Socket accept() const
    {
        // try to accept the connection
        return Socket(_loop, ::accept4(_fd, nullptr, nullptr, SOCK_NONBLOCK));
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
        return ::send(_fd, buf, len, flags);
    }

    /**
     *  Send data to the connection
     * 
     *  This method is directly forwarded to the ::writev() system call
     * 
     *  @param  iov     Array of struct iovec objects
     *  @param  iovcnt  Number of items in the array
     *  @return ssize_t Number of bytes sent
     */
    ssize_t writev(const struct iovec *iov, int iovcnt) const
    {
        return ::writev(_fd, iov, iovcnt);
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
        return ::recv(_fd, buf, len, flags);
    }
    
    /**
     *  Close the socket
     *  @return bool
     */
    bool close()
    {
        // try to close
        if (::close(_fd) < 0 && errno != EBADF) return false;
        
        // forget the filedescriptor
        _fd = -1;
        
        // done
        return true;
    }

};

/**
 *  End namespace
 */
}}

