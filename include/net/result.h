/**
 *  Result.h
 *
 *  Class for storing the result of a cURL request
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React { namespace Curl {

/**
 *  Forward declarations
 */
class Request;
class Result;
class CurlMulti;

/**
 *  DeferredResult declaration
 */
using DeferredResult = Deferred<Result, CurlMulti>;

/**
 *  CURL Result class definition
 */
class Result
{
private:
    /**
     *  Buffer to write the body into
     */
    std::string _body;

    /**
     *  Buffer to write the headers
     */
    std::string _headers;

    /**
     *  Used to store the response code from the remote server
     *  @see http://curl.haxx.se/libcurl/c/curl_easy_getinfo.html#CURLINFORESPONSECODE
     */
    int _status_code;

    /**
     *  Shared pointer to the deferred result, which basically holds the user
     *  specified callbacks
     */
    std::shared_ptr<DeferredResult> _deferred;

    /**
     *  The curl object we have ownership over
     */
    std::shared_ptr<CurlMulti> _curl;

    /**
     *  Buffer for any form of errors
     */
    char _error[CURL_ERROR_SIZE];

    /**
     *  CurlMulti is a good friend that may touch our internals
     */
    friend class CurlMulti;

public:

    /**
     *  Constructor
     *
     *  @param  handle The cURL handle to hook into for the results
     *  @param  curl   CurlMulti object we need to take ownership over
     */
    Result(CURL *handle, const std::shared_ptr<DeferredResult> &callbacks, CurlMulti *curl = nullptr);

    /**
     *  Deconstructor
     */
    virtual ~Result() {}

    /**
     *  Retrieve the body of the result
     *  @return std::string
     */
    const std::string &body() const { return _body; }

    /**
     *  Retrieve the headers of the result
     *  @return std::string
     */
    const std::string &headers() const { return _headers; }

    /**
     *  Retrieve a human readable error
     *  @return const char*
     */
    const char* error() const { return _error; }

    /**
     *  Retrieve the status code of the result
     *  @return int
     */
    int status() const { return _status_code; }
};

/**
 *  End namespace
 */
}}
