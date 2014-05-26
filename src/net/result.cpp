/**
 *  Result.cpp
 *
 *  Class for storing the result of a cURL request
 *
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace React { namespace Curl {

/**
 *  cURL write callback
 */
static size_t onWrite(char *data, size_t size, size_t nmemb, void *userdata)
{
    // Turn our userdata into our Result
    std::string *body = (std::string*) userdata;

    // Append the data to our body buffer
    body->append(data, size * nmemb);

    // Return the amount of bytes processed (all of them)
    return size * nmemb;
}

/**
 *  Constructor
 *
 *  @param  handle The cURL handle to hook into for the results
 */
Result::Result(CURL *handle, const std::shared_ptr<DeferredResult> &callbacks, CurlMulti *curl)
: _deferred(callbacks)
, _curl(curl)
{
    // Let's set the first byte of the error buffer to \0 so we can at least check if there is an error
    _error[0] = '\0';

    // Set the curl error buffer
    curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, _error);

    // Register our write functions
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, onWrite);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &_body);
    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, onWrite);
    curl_easy_setopt(handle, CURLOPT_WRITEHEADER, &_headers);
}

/**
 *  End namespace
 */
}}
