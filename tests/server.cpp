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
    React::Tcp::Server server(&loop, 8766, [&server]() -> bool {
    
        std::cout << "server is readable" << std::endl;
    
        // create the connection
        React::Tcp::Connection connection(server);
        
        connection.onReadable([]() -> bool {
            
            std::cout << "connection is readable" << std::endl;
            
            return true;
            
        });
        
        connection.onWritable([]() -> bool {
            
            std::cout << "connection is writable" << std::endl;
            
            return true;
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
        
        
        // keep watching for readability
        return true;
    });
    
    // show the server address
    std::cout << server.address() << std::endl;
    
    // run the event loop
    loop.run();
    
    // done
    return 0;
}

