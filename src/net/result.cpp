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
 *  Constructor
 *
 *  @param  handle The cURL handle to hook into for the results
 */
Result::Result(CURL* handle)
{
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, Result::onWrite);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
}

/**
 *  cURL callback function for body data
 */
size_t Result::onWrite(char *data, size_t size, size_t nmemb, void *userdata)
{
    // Turn our userdata into our Result
    Result *result = (Result*) userdata;

    // Append the data to our body buffer
    result->_body.append(data, size * nmemb);

    // Return the amount of bytes processed (all of them)
    return size * nmemb;
}

/**
 *  End namespace
 */
}}
