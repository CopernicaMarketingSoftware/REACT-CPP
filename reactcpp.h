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
#include <fcntl.h>

/**
 *  C++ dependencies
 */
#include <memory>
#include <set>
#include <map>
#include <stdexcept>
#include <iostream>
#include <list>
#include <cstring>

/**
 *  Other include files
 */
#include <reactcpp/exception.h>
#include <reactcpp/types.h>
#include <reactcpp/loop.h>
#include <reactcpp/loopreference.h>
#include <reactcpp/mainloop.h>
#include <reactcpp/worker.h>
#include <reactcpp/watcher.h>
#include <reactcpp/deferred.h>
#include <reactcpp/uint128_t.h>
#include <reactcpp/watchers/read.h>
#include <reactcpp/watchers/write.h>
#include <reactcpp/watchers/signal.h>
#include <reactcpp/watchers/synchronize.h>
#include <reactcpp/watchers/cleanup.h>
#include <reactcpp/watchers/timeout.h>
#include <reactcpp/watchers/interval.h>
#include <reactcpp/watchers/status.h>
#include <reactcpp/fd.h>
#include <reactcpp/pipe.h>
#include <reactcpp/readpipe.h>
#include <reactcpp/writepipe.h>
#include <reactcpp/fullpipe.h>
#include <reactcpp/process.h>
#include <reactcpp/net/ipv4.h>
#include <reactcpp/net/ipv6.h>
#include <reactcpp/net/ip.h>
#include <reactcpp/net/address.h>
#include <reactcpp/dns/iprecord.h>
#include <reactcpp/dns/mxrecord.h>
#include <reactcpp/dns/mxresult.h>
#include <reactcpp/dns/channel.h>
#include <reactcpp/dns/types.h>
#include <reactcpp/dns/base.h>
#include <reactcpp/dns/resolver.h>
#include <reactcpp/tcp/exception.h>
#include <reactcpp/tcp/types.h>
#include <reactcpp/tcp/address.h>
#include <reactcpp/tcp/socketaddress.h>
#include <reactcpp/tcp/peeraddress.h>
#include <reactcpp/tcp/socket.h>
#include <reactcpp/tcp/server.h>
#include <reactcpp/tcp/connection.h>
#include <reactcpp/tcp/buffer.h>
#include <reactcpp/tcp/out.h>
#include <reactcpp/tcp/in.h>

/**
 *  End if
 */
#endif
