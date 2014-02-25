/**
 *  Request.h
 *
 *  Base class for requests
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
class Request
{
private:
    /**
     *  Pointer to the resolver
     *  @param  Base
     */
    Base *_resolver;

protected:
    /**
     *  Constructor
     *  @param  resolver
     */
    Request(Base *resolver) : _resolver(resolver)
    {
        // increment pending calls
        resolver->increment();
    }

public:
    /**
     *  Destructor
     */
    virtual ~Request()
    {
        // decrement pending calls
        _resolver->decrement();
    }
};

/**
 *  End namespace
 */
}}

