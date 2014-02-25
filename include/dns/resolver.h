/**
 *  Resolver.h
 *
 *  Class for resolving domain names
 */

/**
 *  Set up namespace
 */
namespace React { namespace Dns {

/**
 *  Class definition
 */
class Resolver : private Base
{
public:
    /**
     *  Constructor
     *  @param  loop    Event loop
     */
    Resolver(Loop *loop) : Base(loop) {}
    
    /**
     *  Destructor
     */
    virtual ~Resolver() {}
    
    /**
     *  Find all IP addresses for a certain domain
     *  @param  domain      The domain to fetch the IPs for
     *  @param  version     IP version, can be 4 or 6
     *  @param  callback
     *  @return bool
     */
    bool ips(const std::string &domain, int version, const IpsCallback &callback);

    /**
     *  Find all IP addresses for a certain domain using a simpler callback
     *  @param  domain      The domain to fetch the IPs for
     *  @param  version     IP version, can be 4 or 6
     *  @param  callback
     *  @return bool
     */
    bool ips(const std::string &domain, int version, const IpsSimpleCallback &callback) 
    { 
        // pass on the call
        return ips(domain, version, [callback](IpsResult &&ips, const char *error) { 
            
            // call the simple callback
            callback(std::move(ips)); 
        });
    }

    /**
     *  Find all IP addresses for a certain domain
     *  This method fetches all IPs, no matter the version, both Ipv4 and Ipv6
     *  @param  domain      The domain to fetch the IPs for
     *  @param  callback
     *  @return bool
     */
    bool ips(const std::string &domain, const IpsCallback &callback);

    /**
     *  Find all IP addresses for a certain domain using a simpler callback
     *  This method fetches all IPs, no matter the version, both Ipv4 and Ipv6
     *  @param  domain      The domain to fetch the IPs for
     *  @param  callback
     *  @return bool
     */
    bool ips(const std::string &domain, const IpsSimpleCallback &callback) 
    { 
        // pass on the function
        return ips(domain, [callback](IpsResult &&ips, const char *error) { 
            
            // call the simple callback
            callback(std::move(ips)); 
        });
    }

};

/**
 *  End namespace
 */
}}
