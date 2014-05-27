/**
 *  CurlMulti.h
 *
 *  cURL Multi for React event loop, this will do smart tricks to saves on bandwidth
 *  and improve performance
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React { namespace Curl {

/**
 *  Main CURLMulti handle definition
 */
class CurlMulti
{
private:
    /**
     *  cURL multi handler
     */
    CURLM* _handle;

    /**
     *  The event Loop
     */
    Loop *_loop;

    /**
     *  Mapped file descriptors coming from cURL to ReadWatchers, if the file
     *  descriptor is in here you pretty much know that we are watching it.
     */
    std::map<int, std::shared_ptr<ReadWatcher>> _read_watchers;

    /**
     *  Mapped file descriptors coming from cURL to WriteWatchers, if the file
     *  descriptor is in here you pretty much know that we will let you know
     *  when you may write on it.
     */
    std::map<int, std::shared_ptr<WriteWatcher>> _write_watchers;

    /**
     *  The amount of running handles
     */
    int _running = 0;

    /**
     *  This will check if there are curl operations that are finished, if there
     *  are the callbacks attached to it will be executed and it will be removed
     */
    void checkFinished();

    /**
     *  Callback function for cURL where it'll let us know about file descriptors we have
     *  to add or remove
     */
    static int setupSocket(CURL *curl, curl_socket_t fd, int action, void *userdata, void *socketpointer);

    /**
     *  Callback function for cURL where it'll ask us to tickle it in timeout milliseconds.
     */
    static int setupTimeout(CURLM *multi, long timeout, void *userdata);

public:
    /**
     *  Constructor
     *
     *  @param  loop    Event loop
     */
    CurlMulti(Loop *loop);

    /**
     *  Deconstructor
     */
    virtual ~CurlMulti();

    /**
     *  Retrieve the event loop of this cURL multi object
     */
    Loop *loop() const { return _loop; }

    /**
     *  Casting operator to cast the Curl object to a cURL multi handler, used
     *  to add CURL objects to the multi handler.
     *
     *  @return CURLM*
     */
    bool add(CURL *curl_easy)
    {
        return curl_multi_add_handle(_handle, curl_easy) == CURLM_OK;
    }
};

/**
 *  End namespace
 */
}}
