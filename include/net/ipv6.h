/**
 *  Ipv6.h
 *
 *  Class representing an IPv6 address
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React { namespace Net {

/**
 *  IP address class
 */
class Ipv6
{
private:
    /**
     *  The address
     *  @var struct in6_addr
     */
    struct in6_addr _addr;

public:
    /**
     *  Constructor to create an empty invalid address
     */
    Ipv6()
    {
        // set to zero's
        memset(&_addr, 0, sizeof(struct in6_addr));
    }
    
    /**
     *  Copy constructor
     *  @param  ip      Address to copy
     */
    Ipv6(const Ipv6 &ip)
    {
        // copy address
        memcpy(&_addr, &ip._addr, sizeof(struct in6_addr));
    }
    
    /**
     *  Move operator
     *  @param  ip      Address to copy
     */
    Ipv6(Ipv6 &&ip)
    {
        // copy address
        memcpy(&_addr, &ip._addr, sizeof(struct in6_addr));
    }
    
    /**
     *  Construct from a string representation
     *  @param  ip      String representation
     */
    Ipv6(const char *ip)
    {
        // try to parse
        if (inet_pton(AF_INET6, ip, &_addr) == 1) return;

        // failure, set to zero
        memset(&_addr, 0, sizeof(struct in6_addr));
    }

    /**
     *  Construct from a string representation
     *  @param  ip      String representation
     */
    Ipv6(const std::string &ip)
    {
        // try to parse
        if (inet_pton(AF_INET6, ip.c_str(), &_addr) == 1) return;

        // failure, set to zero
        memset(&_addr, 0, sizeof(struct in6_addr));
    }
    
    /**
     *  Construct from a struct in6_addr object
     *  @param  ip      in6_addr storage
     */
    Ipv6(const struct in6_addr ip)
    {
        // copy address
        memcpy(&_addr, &ip, sizeof(struct in6_addr));
    }
    
    /**
     *  Construct from a pointer to a struct in6_addr object
     *  @param  ip      in6_addr storage
     */
    Ipv6(const struct in6_addr *ip)
    {
        // copy address
        memcpy(&_addr, ip, sizeof(struct in6_addr));
    }
    
    /**
     *  Constructor based on ares ipv6 structure
     *  @param  ip      ares_in6_addr storage
     */
    Ipv6(const struct ares_in6_addr ip)
    {
        // copy address just as if it was an in6_addr
        memcpy(&_addr, &ip, sizeof(struct in6_addr));
    }
    
    /**
     *  Constructor based on ares ipv6 structure
     *  @param  ip      ares_in6_addr storage
     */
    Ipv6(const struct ares_in6_addr *ip)
    {
        // copy address just as if it was an in6_addr
        memcpy(&_addr, ip, sizeof(struct in6_addr));
    }
    
    /**
     *  Destructor
     */
    virtual ~Ipv6() {}

    /**
     *  Pointer to the internal address
     *  @return struct in6_addr*
     */
    const struct in6_addr *internal() const
    {
        return &_addr;
    }
    
    /**
     *  Is this address valid?
     *  @return bool
     */
    bool valid() const
    {
        // construct invalid address
        Ipv6 invalid;
        
        // compare if equal to invalid
        return memcmp(&_addr, &invalid._addr, sizeof(struct in6_addr)) != 0;
    }
    
    /**
     *  Assign a different IP address to this object
     *  @param  ip      The other address
     *  @return Ipv6
     */
    Ipv6 &operator=(const Ipv6 &address)
    {
        // skip identicals
        if (&address == this) return *this;
    
        // copy address
        memcpy(&_addr, &address._addr, sizeof(struct in6_addr));

        // done
        return *this;
    }

    /**
     *  Compare two IP addresses
     *  @param  ip      The address to compare
     *  @return bool
     */
    bool operator==(const Ipv6 &address) const
    {
        // compare memory
        return memcmp(&_addr, &address._addr, sizeof(struct in6_addr)) == 0;
    }

    /**
     *  Compare two IP addresses
     *  @param  ip      The address to compare
     *  @return bool
     */
    bool operator!=(const Ipv6 &address) const
    {
        // compare memory
        return memcmp(&_addr, &address._addr, sizeof(struct in6_addr)) != 0;
    }

    /**
     *  Compare two IP addresses
     *  @param  ip      The address to compare
     *  @return bool
     */
    bool operator<(const Ipv6 &address) const
    {
        // compare addresses
        return memcmp(&_addr, &address._addr, sizeof(struct in6_addr)) < 0;
    }

    /**
     *  Compare two IP addresses
     *  @param  ip      The address to compare
     *  @return bool
     */
    bool operator>(const Ipv6 &address) const
    {
        // compare addresses
        return memcmp(&_addr, &address._addr, sizeof(struct in6_addr)) > 0;
    }

    /**
     *  String representation of the address
     *  @return string
     */
    const std::string toString() const
    {
        // not valid?
        if (!valid()) return std::string("::");
        
        // construct a buffer
        char buffer[INET6_ADDRSTRLEN];
        
        // convert
        inet_ntop(AF_INET6, &_addr, buffer, INET6_ADDRSTRLEN);
        
        // done
        return std::string(buffer);
    }
};

/**
 *  End of namespace
 */
}}

