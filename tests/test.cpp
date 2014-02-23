/**
 *  Test.cpp
 * 
 *  Simple test case for the library
 * 
 *  @copyright 2014 Copernica BV
 */
#include <iostream>
#include <reactcpp.h>


int main()
{
    React::Loop loop;
    
//    loop.onInterval(0.5, []() {
//       
//        std::cout << "interval expired" << std::endl;
//        
//    });

    auto timer = loop.onTimeout(1.0, []() {
        
        std::cout << "TIMEOUT" << std::endl;
    });
    
    loop.run();
    
    return 0;
    
}

