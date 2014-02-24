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
class ReadWatcher;
class WriteWatcher;
class TimeoutWatcher;
class IntervalWatcher;
class SynchronizeWatcher;
class SignalWatcher;
class StatusWatcher;

/**
 *  Timestamp type is a wrapper around libev
 */
using Timestamp = ev_tstamp;
using ReadCallback = Callback<ReadWatcher>;
using WriteCallback = Callback<WriteWatcher>;
using TimeoutCallback = Callback<TimeoutWatcher>;
using IntervalCallback = Callback<IntervalWatcher>;
using SynchronizeCallback = Callback<SynchronizeWatcher>;
using SignalCallback = Callback<SignalWatcher>;
using StatusCallback = Callback<StatusWatcher>;

/**
 *  End namespace
 */
}

