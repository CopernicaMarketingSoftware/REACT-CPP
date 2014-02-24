/**
 *  Ip.h
 *
 *  Ip address class
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React { namespace Net {

/**
 *  Ip address class
 */
class Ip
{
private:
    /**
     *  IP version, can be 4 or 6
     *  @var    char
     */
    char _version = 4;
    
    /**
     *  Union with the actual IP address
     *  @var    union
     */
    union IP {
        // alternatives
        Ipv4 v4;
        Ipv6 v6;
        
        // constructors
        IP() : v4() {}
        IP(const Ipv4 &ip) : v4(ip) {}
        IP(const Ipv6 &ip) : v6(ip) {}
        IP(const Ip &ip) { 
            if (ip._version == 4) v4 = ip._ip.v4;
            if (ip._version == 6) v6 = ip._ip.v6;
        }
        
        // destructor
        ~IP() {}
        
        // assignment operators
        IP &operator=(const Ipv4 &ip) { v4 = ip; return *this; }
        IP &operator=(const Ipv6 &ip) { v6 = ip; return *this; }
    } _ip;

public:
    /**
     *  Empty constructor
     */
    Ip() {}
    
    /**
     *  Constructor based on an other IP address
     *  @param  ip
     */
    Ip(const Ipv4 &ip) : _version(4), _ip(ip) {}
    Ip(const Ipv6 &ip) : _version(6), _ip(ip) {}
    Ip(const Ip &ip) : _version(ip._version), _ip(ip) {}

    /**
     *  Destructor
     */
    virtual ~Ip() {}
    
};

/**
 *  End namespace
 */
}}

