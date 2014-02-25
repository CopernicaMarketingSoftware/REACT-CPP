/**
 *  IpsRequest.h
 *
 *  Class that contains all information for an IPv4 request
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React { namespace Dns {
    
/**
 *  Class definition
 */
class IpsRequest : public Request
{
private:
    /**
     *  The callback to be called
     *  @var    IpsCallback
     */
    IpsCallback _callback;

public:
    /**
     *  Constructor
     *  @param  resolver
     *  @param  callback
     */
    IpsRequest(Base *resolver, const IpsCallback &callback) : 
        Request(resolver), _callback(callback) {}
    
    /**
     *  Destructor
     */
    virtual ~IpsRequest() {}
    
    /**
     *  Invoke the callback
     *  @param  result          The result to report
     *  @param  error           Optional error message
     */
    void invoke(IpsResult &&result, const char *error)
    {
        _callback(std::move(result), error);
    }
};
    
/**
 *  End namespace
 */
}}
