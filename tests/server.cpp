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
    React::Tcp::Server server(&loop, "192.168.1.203", 8765, [&server](React::ReadWatcher *watcher) {
    
        // create the connection
//        React::Tcp::Connection connection(server);
//        
//        connection.onReadable([]() {
//            
//            
//        });
//        
//        connection.onWritable([]() {
//            
//            
//        });
//        
//        connection.onReceived([](const char *data, size_t size) {
//            
//            
//        });
//        
//        connection.onClosed([]() {
//            
//            
//        });
        
        
//        std::cout << server.address() << std::endl;
    
    
    
    });
    
    // show the server address
    std::cout << server.address() << std::endl;
    
    // run the event loop
    loop.run();
    
    // done
    return 0;
}

