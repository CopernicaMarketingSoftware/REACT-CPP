/**
 *  Server.cpp
 *
 *  Test program to start a server application
 *
 *  @copyright 2014 Copernica BV
 */
#include <reactcpp.h>
#include <iostream>

/**
 *  Main program
 */
int main()
{
    // we need a main loop
    React::MainLoop loop;

    // and a TCP server
    React::Tcp::Server server(&loop, 8765, [](React::ReadWatcher *watcher) {
    
    
    
    });
    
    // show the server address
    std::cout << server.address() << std::endl;
    
    // run the event loop
    loop.run();
    
    // done
    return 0;
}

