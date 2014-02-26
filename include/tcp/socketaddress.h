/**
 *  SocketAddress.h
 *
 *  Extension to the Net::Address class that can be directly applied to the
 *  socket and that will fetch the local socket address
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
class SocketAddress : public Net::Address
{
private:
    /**
     *  Helper function that instantiates a Net::Address
     *  @param  fd      Filedescriptor
     *  @return Net::Address
     */
    Net::Address instantiate(int fd) const
    {
        // create a union that holds all possible values
        union {
            struct sockaddr s;
            struct sockaddr_in v4;
            struct sockaddr_in6 v6;
        } u;
        
        // size of the buffer
        socklen_t slen = sizeof(u); 
     
        // check for success
        if (getsockname(fd, &u.s, &slen) == 0)
        {
            // check family
            if (u.s.sa_family == AF_INET6) return Net::Address(Net::Ip(u.v6.sin6_addr), ntohs(u.v6.sin6_port));
            if (u.s.sa_family == AF_INET) return Net::Address(Net::Ip(u.v4.sin_addr), ntohs(u.v4.sin_port));
        }
    
        // default invalid address
        return Net::Address();
    }

public:
    /**
     *  Constructor
     *  @param  fd      Filedescriptor
     */
    SocketAddress(int fd) : Net::Address(instantiate(fd)) {}
    
    /**
     *  Constructor
     *  @param  socket  The socket
     */
    SocketAddress(const Socket &socket) : SocketAddress(socket.fd()) {}
    
    /**
     *  Constructor
     *  @param  socket  The socket
     */
    SocketAddress(const Socket *socket) : SocketAddress(socket->fd()) {}
    
    /**
     *  Destructor
     */
    virtual ~SocketAddress() {}
};

/**
 *  End namespace
 */
}}

