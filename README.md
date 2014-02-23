REACT-CPP
=========

REACT-CPP is an event loop library that fully utilizes the new C++11 lambda 
functions to notify you when there is activity on a filedescriptor or on a 
timer.


EVENT LOOP
==========

The React::Loop and the React::MainLoop classes are the central classes
of this library, and they represent the actual event loop. These are the classes
that have methods to set timers and to register callback functions that will
be called when a filedescriptor becomes readable or writable.

There is a subtle difference between the React::MainLoop and React::Loop
classes. The React::MainLoop is supposed to run the main event loop for the 
entire application, while the React::Loop classes are additional event loops
that you can (for example) use in additional threads. In normal circumstances, 
you should never instantiate more than once instance of the React::MainLoop 
class, while it is perfectly legal to instantiate as many React::Loop objects
as you wish.

Because the React::MainLoop class is intended to control the entire application,
it has some additional methods to register signal handlers. Such methods are
not available in the regular React::Loop class.

In a typical application you create an instance of the mainloop class, and
then you register filedescriptors that you'd like to watch for readability,
register event handlers and timers:

````c++
#include <reactcpp.h>
#include <unistd.h>
#include <iostream>

/**
 *  Main application procedure
 *  @return int
 */
int main()
{
    // create an event loop
    React::MainLoop loop;
    
    // we'd like to be notified when input is available on stdin
    loop.onReadable(STDIN_FILENO, []() {
    
        // read input
        std::string buffer;
        std::cin >> buffer;
    
        // show what we read
        std::cout << buffer << std::endl;
    });

    // set a timer to stop the application after five seconds
    loop.onTimeout(5.0, []() {
    
        // report that the timer expired
        std::cout << "timer expired" << std::endl;
    
        // stop the application
        exit(0);
    });
    
    // handler when control+c is pressed
    loop.onSignal(SIGINT, []() {
        
        // report that we got a signal
        std::cout << "control+c detected" << std::endl;
        
        // stop the application
        exit(0);
    });

    // run the event loop
    loop.run();

    // done
    return 0;
}
````

The above example contains a very simple echo application. Everything that
the application reads from stdin is directly echo'd back to stdout. After five
seconds the application automatically stops, and when the SIGINT signal is 
caught, the application will also exit.
