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
     *  The curl handle that we purely use to build the request
     */
    CURL *_builder;

    /**
     *  The event loop the request will be made on
     */
    Loop *_loop;

    /**
     *  The user specified CurlMulti handler, if this is a nullptr we create a temporary
     *  one for them
     */
    CurlMulti *_multi_handler = nullptr;

public:
    /**
     *  Constructor
     *
     *  @param  loop    Event loop
     *  @param  url     Request url
     */
    Request(Loop *loop, const std::string &url);

    /**
     *  Constructor
     *
     *  @param multi_handler   The cURL multi handler
     *  @param url             Request url
     */
    Request(CurlMulti *multi_handler, const std::string &url);

    /**
     *  Deconstructor
     */
    virtual ~Request();

    /**
     *  Allows you to set curl specify options, setting CURLOPT_PRIVATE is not
     *  allowed as we use this internally
     *
     *  @see http://curl.haxx.se/libcurl/c/curl_easy_setopt.html
     *  @return True if succesful, false otherwise
     */
    template <class ...Arguments>
    bool setopt(CURLoption option, Arguments ...parameters)
    {
        // If the user is trying to set one of the following options return false, as we use them internally
        if (option == CURLOPT_PRIVATE) return false;

        // Just redirect everything into curl_easy_setopt
        return curl_easy_setopt(_builder, option, parameters...) == CURLE_OK;
    }

    /**
     *  Actually execute the request
     *
     *  @return DeferredResult     Which you can then use to set callbacks to get the final result
     */
    DeferredResult& execute();
};

/**
 *  End namespace
 */
}}
