/**
 *  Curl.cpp
 *
 *  Main CURL handle for React event loop
 *
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace React { namespace Curl {

/**
 *  Static map which maps our curl objects to loops
 *  @todo Somehow remove our Curl objects upon destruction of the Loop objects.
 */
static std::map<Loop*, std::unique_ptr<Curl>> _instances;

/**
 *  Get the Curl object associated with the specified Loop
 */
Curl *Curl::get(Loop *loop)
{
    // Look for loop in our _instances map
    auto iter = _instances.find(loop);

    // If we found what we were looking for return it
    if (iter != _instances.end()) return iter->second.get();

    // If we didn't find it, create it, append it and return it
    Curl *output = new Curl(loop);
    _instances[loop] = std::unique_ptr<Curl>(output); // Turn it into a unique_ptr
    return output;
}

/**
 *  Constructor
 */
Curl::Curl(Loop* loop)
: _handle(curl_multi_init())
, _loop(loop)
{
    curl_multi_setopt(_handle, CURLMOPT_SOCKETFUNCTION, Curl::setupSocket);
    curl_multi_setopt(_handle, CURLMOPT_SOCKETDATA, this);
    curl_multi_setopt(_handle, CURLMOPT_TIMERFUNCTION, Curl::setupTimeout);
    curl_multi_setopt(_handle, CURLMOPT_TIMERDATA, this);
}

/**
 *  Deconstructor
 */
Curl::~Curl()
{
    if (_handle) curl_multi_cleanup(_handle);
}

/**
 *  Callback function for cURL where it'll let us know about file descriptors we have
 *  to add or remove
 */
int Curl::setupSocket(CURL *curl_easy, curl_socket_t fd, int action, void *userdata, void *socketpointer)
{
    Curl *curl = (Curl*) userdata;
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
                curl->_read_watchers[fd] = curl->_loop->onReadable(fd, [curl, fd, action]() {
                    // tell cURL that it may read from fd
                    curl_multi_socket_action(curl->_handle, fd, action, NULL);
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
                curl->_write_watchers[fd] = curl->_loop->onWritable(fd, [curl, fd, action]() {
                    // tell cURL that it may write to fd
                    curl_multi_socket_action(curl->_handle, fd, action, NULL);
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
 */
int Curl::setupTimeout(CURLM *multi, long timeout, void *userdata)
{
    Curl *curl = (Curl*) userdata;
    curl->_loop->onTimeout(timeout/1000, [multi]() {
        curl_multi_socket_action(multi, CURL_SOCKET_TIMEOUT, 0, NULL);
    });
    // cURL documentation says that we must return 0 from here
    return 0;
}

/**
 *  End namespace
 */
}}
