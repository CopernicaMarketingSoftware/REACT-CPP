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

/**
 *  CURL class definition
 */
class Result
{
private:
    /**
     *  Buffer to write the body into
     */
    std::string _body;

    /**
      *  cURL callback function for body data
      */
    static size_t onWrite(char *data, size_t size, size_t nmemb, void *userdata);

public:
    /**
     *  Constructor
     *
     *  @param  handle The cURL handle to hook into for the results
     */
    Result(CURL* handle);

    /**
     *  Deconstructor
     */
    virtual ~Result() {}
};

using DeferredResult = Deferred<Result, Request>;

/**
 *  End namespace
 */
}}
