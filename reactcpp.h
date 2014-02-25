/**
 *  ReactCpp.h
 * 
 *  Main header file for the REACT-CPP wrapper library
 * 
 *  @copyright 2014 Copernica BV
 */
#ifndef REACTCPP_H
#define REACTCPP_H

/**
 *  C dependencies
 */
#include <ev.h>
#include <ares.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

/**
 *  C++ dependencies
 */
#include <memory>
#include <set>
#include <map>

/**
 *  Other include files
 */
#include <reactcpp/exception.h>
#include <reactcpp/callback.h>
#include <reactcpp/types.h>
#include <reactcpp/loop.h>
#include <reactcpp/mainloop.h>
#include <reactcpp/worker.h>
#include <reactcpp/watcher.h>
#include <reactcpp/watchers/read.h>
#include <reactcpp/watchers/write.h>
#include <reactcpp/watchers/signal.h>
#include <reactcpp/watchers/synchronize.h>
#include <reactcpp/watchers/timeout.h>
#include <reactcpp/watchers/interval.h>
#include <reactcpp/watchers/status.h>
#include <reactcpp/net/ipv4.h>
#include <reactcpp/net/ipv6.h>
#include <reactcpp/net/ip.h>
#include <reactcpp/net/address.h>
#include <reactcpp/dns/iprecord.h>
#include <reactcpp/dns/types.h>
#include <reactcpp/dns/base.h>
#include <reactcpp/dns/resolver.h>
#include <reactcpp/tcp/exception.h>
#include <reactcpp/tcp/types.h>
#include <reactcpp/tcp/socket.h>
#include <reactcpp/tcp/socketaddress.h>
#include <reactcpp/tcp/server.h>

/**
 *  End if
 */
#endif
