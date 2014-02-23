/**
 *  Watcher.h
 *
 *  Internal base class for the watcher objects (timers, readers, writers, synchronizers)
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React {

/**
 *  Class definition
 */
class Watcher
{
public:
    /**
     *  Invoke the watcher
     */
    virtual void invoke() = 0;
};

/**
 *  End namespace
 */
}
