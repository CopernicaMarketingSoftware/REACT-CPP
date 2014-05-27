/**
 *  Request.cpp
 *
 *  Class to make requests using cURL
 *
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace React { namespace Curl {

/**
 *  Constructor
 *
 *  @param  loop    Event loop
 *  @param  url     Request url
 */
Request::Request(Loop *loop, const std::string &url)
: _builder(curl_easy_init())
, _loop(loop)
{
    setopt(CURLOPT_URL, url.c_str()); // set the url
    // Set default curl options here
    setopt(CURLOPT_FOLLOWLOCATION, 1L); // follow any form of redirects automatically
    setopt(CURLOPT_MAXREDIRS, 10L); // limit the amount of redirects to 10 to avoid infinite redirects
    setopt(CURLOPT_NOSIGNAL, 1L);
    //setopt(CURLOPT_VERBOSE, 1L); // Uncomment this line in case of debugging curl
}

/**
 *  Constructor
 *
 *  @param  multi_handler   CurlMulti object to make the requests on
 *  @param  url             Request url
 */
Request::Request(CurlMulti *multi_handler, const std::string &url)
: Request(multi_handler->loop(), url)
{
    _multi_handler = multi_handler;
}

/**
 *  Deconstructor
 */
Request::~Request()
{
    // Clean our builder up
    if (_builder) curl_easy_cleanup(_builder);
}

/**
 *  Actually execute the request
 *
 *  @return DeferredResult     Which you can then use to set callbacks to get the final result
 */
DeferredResult& Request::execute()
{
    // Duplicate our builder
    CURL *handle = curl_easy_duphandle(_builder);

    // create the deferred result
    auto deferred = std::make_shared<DeferredResult>();

    CurlMulti *curl = nullptr;

    // Create a curl multi object if no multi handler is attached to this request
    // as we need to make the request asynchronous, we'll hand over ownership
    // to the Result we'll be creating just a bit later
    if (_multi_handler == nullptr) curl = new CurlMulti(_loop);

    // Create a Result for our handle, this is a raw pointer as we are saving it
    // in a cURL structure, which is in C.
    Result *result = new Result(handle, deferred, curl);

    // If curl is null we obviously have a real multi handler, so just assign that for future operations
    if (curl == nullptr) curl = _multi_handler;

    // Assign our result to CURLOPT_PRIVATE
    curl_easy_setopt(handle, CURLOPT_PRIVATE, result);

    // and add our handle to the multi handler so it will actually start.
    curl->add(handle);

    // return the deferred result
    return *deferred;
}

/**
 *  End namespace
 */
}}
