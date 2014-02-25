/**
 *  Resolver.cpp
 *
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace React { namespace Dns {

/**
 *  Callback method that is called when a resolver call for IPv4 addresses completed
 *  @param  data    User-supplied-data (pointer to the resolver-request)
 *  @param  status  Status of the resolver
 *  @param  timeout Number of times that a query timed out
 *  @param  buffer  The answer buffer
 *  @param  len     Length of the answer buffer
 */
static void ipv4callback(void *data, int status, int timeouts, unsigned char *buffer, int len)
{
    // retrieve pointer to the request (and put it in a unique ptr so that it will be destructed)
    auto request = std::unique_ptr<IpsRequest>((IpsRequest *)data);
    
    // check result
    if (status == ARES_SUCCESS)
    {
        // report success
        request->invoke(Ipv4Result(buffer, len), nullptr);
    }
    else
    {
        // report failure
        request->invoke(IpsResult(), ares_strerror(status));
    }
}

/**
 *  Callback method that is called when a resolver call for IPv6 addresses completed
 *  @param  data    User-supplied-data (pointer to the resolver-request)
 *  @param  status  Status of the resolver
 *  @param  timeout Number of times that a query timed out
 *  @param  buffer  The answer buffer
 *  @param  len     Length of the answer buffer
 */
static void ipv6callback(void *data, int status, int timeouts, unsigned char *buffer, int len)
{
    // retrieve pointer to the request (and put it in a unique ptr so that it will be destructed)
    auto request = std::unique_ptr<IpsRequest>((IpsRequest *)data);
    
    // check result
    if (status == ARES_SUCCESS)
    {
        // report success
        request->invoke(Ipv6Result(buffer, len), nullptr);
    }
    else
    {
        // report failure
        request->invoke(IpsResult(), ares_strerror(status));
    }
}

/**
 *  Resolve IPv4 addresses
 *  @param  name            The domain to resolve
 *  @param  version         IP version
 *  @param  callback        Callback that is called once the addresses are resolved
 *  @return bool
 */
bool Resolver::ips(const std::string &domain, int version, const IpsCallback &callback)
{
    // channel should be valid
    if (!_channel) return false;
    
    // construct request object
    auto *request = new IpsRequest(this, callback);
    
    // what version do we need?
    if (version == 6)
    {
        // tell the ares library to run the AAAA query
        ares_query(_channel, domain.c_str(), ns_c_in, ns_t_aaaa, ipv6callback, request);
    }
    else if (version == 4)
    {
        // tell the ares library to run the A query
        ares_query(_channel, domain.c_str(), ns_c_in, ns_t_a, ipv4callback, request);
    }
    else
    {
        // error
        return false;
    }
    
    // set a new timeout
    setTimeout();
    
    // done
    return true;
}

/**
 *  Find all IP addresses for a certain domain
 *  This method fetches all IPs, no matter the version, both Ipv4 and Ipv6
 *  @param  domain      The domain to fetch the IPs for
 *  @param  callback
 *  @return bool
 */
bool Resolver::ips(const std::string &domain, const IpsCallback &callback)
{
    // channel should be valid
    if (!_channel) return false;
    
    // construct result object
    auto results = std::shared_ptr<AllIpsResult>(new AllIpsResult());
 
    // the callback function
    auto helper = [results, callback](IpsResult &&result, const char *error) {
        
        // merge the sets
        results->insert(result.begin(), result.end());
        
        // report the error
        if (error) results->setError(error);
        
        // check if anything is still pending
        if (results->pending(-1) > 0) return;
        
        // pending calls are ready, proceed
        callback(std::move(*results), results->error());
    };
    
    // fetch the IPv4 and IPv6 objects
    if (ips(domain, 6, helper)) results->pending(+1);
    if (ips(domain, 4, helper)) results->pending(+1);
    
    // check if anything is pending
    return results->pending() > 0;
}

/**
 *  End of namespace
 */
}}

