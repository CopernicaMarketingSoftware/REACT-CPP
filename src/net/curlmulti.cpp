/**
 *  CurlMulti.cpp
 *
 *  cURL Multi for React event loop, this will do smart tricks to saves on bandwidth
 *  and improve performance
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
 *  @param  loop         Event loop
 */
CurlMulti::CurlMulti(Loop* loop)
: _handle(curl_multi_init())
, _loop(loop)
{
    curl_multi_setopt(_handle, CURLMOPT_SOCKETFUNCTION, CurlMulti::setupSocket);
    curl_multi_setopt(_handle, CURLMOPT_SOCKETDATA, this);
    curl_multi_setopt(_handle, CURLMOPT_TIMERFUNCTION, CurlMulti::setupTimeout);
    curl_multi_setopt(_handle, CURLMOPT_TIMERDATA, this);
}

/**
 *  Deconstructor
 */
CurlMulti::~CurlMulti()
{
    if (_handle) curl_multi_cleanup(_handle);
}

/**
 *  This will check if there are curl operations that are finished, if there
 *  are the callbacks attached to it will be executed and it will be removed
 */
void CurlMulti::checkFinished()
{
    // Loop through all the handlers of the multi handle
    CURLMsg *msg = nullptr;
    int handlers = 0;

    while ((msg = curl_multi_info_read(_handle, &handlers)))
    {
        if (msg->msg == CURLMSG_DONE)
        {
            Result *result = nullptr;

            if (curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &result) == CURLE_OK && result != nullptr)
            {
                // In case we had an error call the failure() callback, success otherwise
                const char *error = result->error();

                if (*error)
                {
                    // Call the failure callback using the error
                    result->_deferred->failure(error);
                }
                else
                {
                    // If we are succesful we need some extra data
                    long status = -1;
                    CURLcode ret = curl_easy_getinfo(msg->easy_handle, CURLINFO_RESPONSE_CODE, &status);

                    if (ret == CURLE_OK)
                    {
                        // Store this extra data in our result and call the success callback
                        result->_status_code = (int) status;
                        result->_deferred->success(*result);
                    }
                    else
                    {
                        // Call the failure callback with a human readable version of the output of curl_easy_getinfo
                        // @todo This might fail with protocols that don't send a status code?
                        result->_deferred->failure(curl_easy_strerror(ret));
                    }
                }

                // Clean everything up
                curl_multi_remove_handle(_handle, msg->easy_handle);
                curl_easy_cleanup(msg->easy_handle);
                delete result;
            }
        }
    }
}

/**
 *  Callback function for cURL where it'll let us know about file descriptors we have
 *  to add or remove
 *
 *  @param  curl_easy     The cURL handle this socket currently belongs to
 *  @param  fd            The file descriptor to monitor for action
 *  @param  action        The actions to look out for, this is a bitmask
 *  @param  userdata      User specified data, we are assuming a CurlMulti* here
 *  @param  socketpointer A pointer specify to this file descriptor, unused by us
 *
 *  @see http://curl.haxx.se/libcurl/c/curl_multi_socket_action.html
 */
int CurlMulti::setupSocket(CURL *curl_easy, curl_socket_t fd, int action, void *userdata, void *socketpointer)
{
    // Cast userdata to a CurlMulti*
    CurlMulti *curl = static_cast<CurlMulti*>(userdata);

    if (action == CURL_POLL_REMOVE)
    {
        // look for the file descriptor
        auto read_iter = curl->_read_watchers.find(fd);
        if (read_iter != curl->_read_watchers.end())
        {
            // cancel the watcher and remove it from the map
            read_iter->second->cancel();
            curl->_read_watchers.erase(read_iter);
        }
        // look for the file descriptor
        auto write_iter = curl->_write_watchers.find(fd);
        if (write_iter != curl->_write_watchers.end())
        {
            // cancel the watcher and remove it from the map
            write_iter->second->cancel();
            curl->_write_watchers.erase(write_iter);
        }
    }
    else
    {
        // Add onReadable callback
        if (action & CURL_POLL_IN)
        {
            auto iter = curl->_read_watchers.find(fd);
            if (iter == curl->_read_watchers.end())
            {
                curl->_read_watchers[fd] = curl->_loop->onReadable(fd, [curl, fd]() {
                    // tell cURL that it may read from fd
                    curl_multi_socket_action(curl->_handle, fd, CURL_POLL_IN, &curl->_running);
                    curl->checkFinished();
                    return true;
                });
            }
        }
        // Add onWriteable callback
        if (action & CURL_POLL_OUT)
        {
            auto iter = curl->_write_watchers.find(fd);
            if (iter == curl->_write_watchers.end())
            {
                curl->_write_watchers[fd] = curl->_loop->onWritable(fd, [curl, fd]() {
                    // tell cURL that it may write to fd
                    curl_multi_socket_action(curl->_handle, fd, CURL_POLL_OUT, &curl->_running);
                    curl->checkFinished();
                    return true;
                });
            }
        }
    }

    // cURL documentation says that we must return 0 from here
    return 0;
}

/**
 *  Callback function for cURL where it'll ask us to tickle it in timeout milliseconds.
 *  @param  multi     The cURL multi object we need to apply a timeout on
 *  @param  timeout   The amount of milliseconds we have to wait before tickling multi
 *  @param  userdata  User specified data, a CurlMulti* in our case
 */
int CurlMulti::setupTimeout(CURLM *multi, long timeout, void *userdata)
{
    // Cast our userdata
    CurlMulti *curl = static_cast<CurlMulti*>(userdata);

    // Declare our callback
    auto callback = [curl]() {
        curl_multi_socket_action(curl->_handle, CURL_SOCKET_TIMEOUT, 0, &curl->_running);
    };

    // If the timeout is over 0 schedule it
    if (timeout > 0) curl->_loop->onTimeout(timeout/1000, callback);
    else callback();

    // cURL documentation says that we must return 0 from here
    return 0;
}

/**
 *  End namespace
 */
}}
