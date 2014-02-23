/**
 *  Types.h
 *
 *  File containing all type definitions for the LIBEV-CPP library
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React {

/**
 *  Forward declarations
 */
class Reader;
class Writer;
class Timer;
class Interval;
class Synchronizer;
class Signal;

/**
 *  Timestamp type is a wrapper around libev
 */
using Timestamp = ev_tstamp;
using Callback = std::function<void()>;
using ReadCallback = std::function<void(Reader *)>;
using WriteCallback = std::function<void(Writer *)>;
using TimerCallback = std::function<void(Timer *)>;
using IntervalCallback = std::function<void(Interval *)>;
using SynchronizeCallback = std::function<void(Synchronizer *)>;
using SignalCallback = std::function<void(Signal *)>;

/**
 *  End namespace
 */
}

