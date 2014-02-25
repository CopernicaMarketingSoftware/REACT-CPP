/**
 *  Server.h
 *
 *  A TCP server, that accepts incoming connections
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
class Server
{
private:
    /**
     *  We need a socket
     *  @var    Socket
     */
    Socket _socket;
    
public:
    /**
     *  Constructor to listen to a specific port on a specific IP
     *  @param  loop        Event loop
     *  @param  ip          IP address to listen to
     *  @param  port        Port number to listen to
     *  @param  callback    Function that is called for each incoming connection
     */
    template <typename CALLBACK>
    Server(Loop *loop, const Net::Ip &ip, uint16_t port, const CALLBACK &callback) :
        _socket(loop, ip, port)
    {
        // listen to the socket
        if (!_socket.listen()) throw Exception(strerror(errno));
        
        // install handler when readable
        _socket.onReadable(callback);
    }

    /**
     *  Constructor to listen to a random port on a specific IP
     *  @param  loop        Event loop
     *  @param  ip          IP address to listen to
     *  @param  callback    Function that is called for each incoming connection
     */
    template <typename CALLBACK>
    Server(Loop *loop, const Net::Ip &ip, const CALLBACK &callback) : 
        Server(loop, ip, 0, callback) {}

    /**
     *  Constructor to listen to a specific port
     *  @param  loop        Event loop
     *  @param  port        Port number to listen to
     *  @param  callback    Function that is called for each incoming connection
     */
    template <typename CALLBACK>
    Server(Loop *loop, uint16_t port, const CALLBACK &callback) : 
        Server(loop, Net::Ip(), port, callback) {}
    
    /**
     *  Constructor to listen to a random port
     *  @param  loop        Event loop
     *  @param  callback    Function that is called for each incoming connection
     */
    template <typename CALLBACK>
    Server(Loop *loop, const CALLBACK &callback) : 
        Server(loop, Net::Ip(), 0, callback) {}
    
    /**
     *  Destructor
     */
    virtual ~Server() {}
    
    /**
     *  Retrieve the address to which the server is listening
     *  @return Net::Address
     */
    Net::Address address() const
    {
        // return the socket address
        return SocketAddress(_socket);
    }
    
    /**
     *  Retrieve the IP address to which it is listening
     *  @return Net::Ip
     */
    Net::Ip ip() const
    {
        // fetch the ip from the socket address
        return SocketAddress(_socket).ip();
    }
    
    /**
     *  Retrieve the port number to which the server is listening
     *  @return uint16_t
     */
    uint16_t port() const
    {
        // fetch the port number from the socket address
        return SocketAddress(_socket).port();
    }
};

/**
 *  End namespace
 */
}}
