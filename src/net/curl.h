/**
 *  Curl.h
 *
 *  Main CURL handle for React event loop
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React { namespace Curl {

/**
 *  Main CURL handle definition
 */
class Curl
{
private:
    /**
     *  cURL multi handler
     */
    CURLM* _handle = nullptr;

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
     *  Callback function for cURL where it'll let us know about file descriptors we have
     *  to add or remove
     */
    static int setupSocket(CURL *curl, curl_socket_t fd, int action, void *userdata, void *socketpointer);

    /**
     *  Callback function for cURL where it'll ask us to tickle it in timeout milliseconds.
     */
    static int setupTimeout(CURLM *multi, long timeout, void *userdata);

protected:
    /**
     *  Constructor
     */
    Curl(Loop *loop);
public:
    /**
     *  Get the Curl object associated with the specified Loop
     */
    static Curl* get(Loop *loop);

    /**
     *  Deconstructor
     */
    virtual ~Curl();
};

/**
 *  End namespace
 */
}}
