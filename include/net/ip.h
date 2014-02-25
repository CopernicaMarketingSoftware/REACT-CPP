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
  
    /**
     *  Assign a different IP
     *  @param  that
     *  @return IP
     */
    Ip &operator=(const Ip &that)
    {
        // skip self assignment
        if (this == &that) return *this;
        
        // copy version
        _version = that._version;
        
        // check version
        switch (_version) {
        case 4: _ip.v4 = that._ip.v4; break;
        case 6: _ip.v6 = that._ip.v6; break;
        }
        
        // done
        return *this;
    }
    
    /**
     *  IP version
     *  @return char
     */
    char version() const
    {
        return _version;
    }
    
    /**
     *  Retrieve the IPv4 version
     *  @return Ipv4
     */
    const Ipv4 &v4() const
    {
        return _ip.v4;
    }

    /**
     *  Retrieve the IPv6 version
     *  @return Ipv6
     */
    const Ipv6 &v6() const
    {
        return _ip.v6;
    }

    /**
     *  Convert the object to a string
     *  @return string
     */
    const std::string toString() const
    {
        switch (_version) {
        case 4: return _ip.v4.toString();
        case 6: return _ip.v6.toString();
        default:return std::string();
        }
    }
  
    /**
     *  Comparison of two ips
     *  @param  that
     *  @return bool
     */
    bool operator==(const Ip &that) const
    {
        // versions must be equal
        if (_version != that._version) return false;
        
        // compare on version
        switch (_version) {
        case 4: return _ip.v4 == that._ip.v4;
        case 6: return _ip.v6 == that._ip.v6;
        
        // all ips of a different version are considered equal
        default:return true;
        }
    }
    
    /**
     *  Comparison of two ips
     *  @param  that
     *  @return bool
     */
    bool operator!=(const Ip &that) const
    {
        // versions must not be equal
        if (_version == that._version) return true;
        
        // compare on version
        switch (_version) {
        case 4: return _ip.v4 != that._ip.v4;
        case 6: return _ip.v6 != that._ip.v6;

        // all ips of a different version are considered equal
        default:return false;
        }
    }
    
    /**
     *  Comparison of two ips
     *  @param  that
     *  @return bool
     */
    bool operator<(const Ip &that) const
    {
        // versions must be equal
        if (_version != that._version) return _version < that._version;
        
        // compare on version
        switch (_version) {
        case 4: return _ip.v4 < that._ip.v4;
        case 6: return _ip.v6 < that._ip.v6;

        // all ips of a different version are considered equal
        default:return false;
        }
    }

    /**
     *  Comparison of two ips
     *  @param  that
     *  @return bool
     */
    bool operator>(const Ip &that) const
    {
        // versions must be equal
        if (_version != that._version) return _version > that._version;
        
        // compare on version
        switch (_version) {
        case 4: return _ip.v4 > that._ip.v4;
        case 6: return _ip.v6 > that._ip.v6;

        // all ips of a different version are considered equal
        default:return false;
        }
    }
};

/**
 *  Function to write an IP to a stream
 *  @param  os
 *  @param  ip
 *  @return ostream
 */
inline std::ostream &operator<<(std::ostream &os, const Ip &ip)
{
    os << ip.toString();
    return os;
}

/**
 *  End namespace
 */
}}

