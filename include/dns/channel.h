/**
 *  Channel.h
 *
 *  Wrapper class around the ares channel.
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React { namespace Dns {

/**
 *  Class implementation
 */
class Channel
{
private:
    /**
     *  The actual underlying channel
     */
    ares_channel _channel = nullptr;
public:
    /**
     *  Constructor
     *
     *  Creates the channel
     *
     *  @param  options     the channel options
     *  @param  mask        option mask
     */
    Channel(ares_options& options, int mask)
    {
        // initialize the channel
        ares_init_options(&_channel, &options, mask);
    }

    /**
     *  Destructor
     */
    virtual ~Channel()
    {
        // if there is no channel we have nothing to cleanup
        if (!_channel) return;

        // cancel all current requests
        ares_cancel(_channel);

        // destroy the channel
        ares_destroy(_channel);
    }

    /**
     *  Check whether the channel is valid
     */
    operator bool ()
    {
        // valid when not a null pointer
        return _channel;
    }

    /**
     *  Cast to an ares channel
     */
    operator ares_channel ()
    {
        // return the channel
        return _channel;
    }
};

/**
 *  End namespace
 */
}}
