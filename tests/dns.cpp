/**
 *  Dns.cpp
 *
 *  Test program for the resolver
 *
 *  @copyright 2014 Copernica BV
 */
#include <reactcpp.h>
#include <iostream>

/**
 *  Main procedure
 *  @return int
 */
int main()
{
    // we need (of course) an event loop
    React::MainLoop loop;

    // and a resolver
    React::Dns::Resolver resolver(&loop);
    
    // fetch all IP's
    // @todo also support nog passing errors
    resolver.ips("www.copernica.com", [](React::Dns::IpsResult &&ips) {
        
        for (auto &ip : ips) std::cout << ip << std::endl;
    });
    
    // run the event loop
    loop.run();

    // done
    return 0;
}
