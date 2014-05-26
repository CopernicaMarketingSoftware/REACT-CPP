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
     *  Shared pointer to the deferred result, which basically holds the user
     *  specified callbacks
     */
    std::shared_ptr<DeferredResult> _deferred;

    /**
     *  The curl object we have ownership over
     */
    std::shared_ptr<CurlMulti> _curl;

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

    std::shared_ptr<DeferredResult> deferred() const { return _deferred; }
};

/**
 *  End namespace
 */
}}
