/**
 *  Request.h
 *
 *  Class for sending CURL requests
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React { namespace Curl {

/**
 *  CURL class definition
 */
class Request
{
private:
    /**
     *  The curl handle for this request
     */
    CURL* _handle = nullptr;

public:
    /**
     *  Constructor
     *
     *  @param  loop    Event loop
     *  @todo Fill this constructor with default curl options
     */
    Request(Loop *loop, const std::string& url)
    : _handle(curl_easy_init())
    {
        // Set default curl options here
    }

    /**
     *  Deconstructor
     */
    virtual ~Request()
    {
        if (_handle)    curl_easy_cleanup(_handle);
    }

    /**
     *  Allows you to set curl specify options
     *
     *  @see http://curl.haxx.se/libcurl/c/curl_easy_setopt.html
     *  @return True is succesful, false otherwise
     */
    bool setopt(CURLoption option, ...);

    /**
     *  Actually execute the request
     *  @todo Implement
     */
    DeferredResult& execute();
};

/**
 *  End namespace
 */
}}
