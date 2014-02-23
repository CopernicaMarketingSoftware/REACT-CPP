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
using ReadCallback = Callback<Reader>;
using WriteCallback = Callback<Writer>;
using TimerCallback = Callback<Timer>;
using IntervalCallback = Callback<Interval>;
using SynchronizeCallback = Callback<Synchronizer>;
using SignalCallback = Callback<Signal>;

/**
 *  End namespace
 */
}

