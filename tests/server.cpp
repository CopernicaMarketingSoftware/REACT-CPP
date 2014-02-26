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

    // @todo make address reusable

    // and a TCP server
    React::Tcp::Server server(&loop, 8766, [&server](React::ReadWatcher *watcher) {
    
        std::cout << "server is readable" << std::endl;
    
        // create the connection
        React::Tcp::Connection connection(server);
        
        connection.onReadable([]() {
            
            std::cout << "connection is readable" << std::endl;
            
            
        });
        
        connection.onWritable([]() {
            
            std::cout << "connection is writable" << std::endl;
            
            
        });
        
//        connection.onReceived([](const char *data, size_t size) {
//            
//            
//        });
//        
//        connection.onClosed([]() {
//            
//            
//        });
        
        
    
    
    
    });
    
    // show the server address
    std::cout << server.address() << std::endl;
    
    // run the event loop
    loop.run();
    
    // done
    return 0;
}

