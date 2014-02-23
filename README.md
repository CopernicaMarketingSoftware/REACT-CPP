REACT-CPP
=========

REACT-CPP is an event loop library that utilizes the new C++11 lambda functions 
to notify you when there is activity on a filedescriptor or on a timer. 
Internally, it is a wrapper around the libev library, and does therefore also
depend on that library.


EVENT LOOP
==========

The React::Loop and the React::MainLoop classes are the central classes
of this library. These classes have methods to set timers and to register 
callback functions that will be called when a filedescriptor becomes readable 
or writable.

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

    // set a timer to stop the application after five seconds
    loop.onTimeout(5.0, []() {
    
        // report that the timer expired
        std::cout << "timer expired" << std::endl;
    
        // stop the application
        exit(0);
    });
    
    // we'd like to be notified when input is available on stdin
    loop.onReadable(STDIN_FILENO, []() {
    
        // read input
        std::string buffer;
        std::cin >> buffer;
    
        // show what we read
        std::cout << buffer << std::endl;
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
caught, the application also exits.

There is a subtle difference between the React::MainLoop and React::Loop
classes. The React::MainLoop is supposed to run the main event loop for the 
entire application, while the React::Loop classes are additional event loops
that you can (for example) use in additional threads. In normal circumstances, 
you will never have to instantiate more than once instance of the React::MainLoop 
class, while it is perfectly legal to create as many React::Loop objects
as you wish.

Because the React::MainLoop class is intended to control the entire application,
it has some additional methods to register signal handlers. Such methods are
not available in the regular React::Loop class.


DIFFERENT TYPES OF CALLBACKS
============================

In the first example we showed how to install handlers on the loop object.
However, once such a handler is set, the loop will keep calling it every time
a filedescriptor becomes active. But what if you no longer are interested in
these events? In that case you have a number of options to stop a callback 
from being called.

The first one is by using a different type of callback. In the examples above,
we had registered straightforward callbacks that did not take any parameters. 
However, a different type of callback can be registered too: one that takes
a parameter which you can modify to stop subsequent calls to the callback.

````c++
#include <reactcpp.h>
#include <unistd.h>
#include <iostream>

int main()
{
    // create the event loop
    React::MainLoop loop;
    
    // we'd like to be notified when input is available on stdin
    loop.onReadable(STDIN_FILENO, [](React::Reader *reader) {
    
        // read input
        std::string buffer;
        std::cin >> buffer;
    
        // show what we read
        std::cout << buffer << std::endl;
        
        // from this moment on, we no longer want to receive updates
        reader->cancel();
    });
    
    // run the event loop
    loop.run();
    
    // done
    return 0;
}
````

The program above is only interested in read events until the first line
from stdin has been read. After that it calls the cancel() method on the
React::Reader object, to inform the event loop that it is no longer interested
in read events.

In the above example, this also means that the program automatically exits after
the first line has been read. The reason for this is that the run() method of 
the React::Loop and React::MainLoop classes automatically stop running when 
there are no more callback functions active. By calling the Reader::cancel()
method, the last and only registered callback function is cancelled, and the
event loop has nothing left to monitor.

The type of the parameter that is passed to your callback function depends on
the type of callback function that you have registered.

````c++
loop.onReadable(fd, [](React::Reader *reader) { ... });
loop.onWritable(fd, [](React::Writer *writer) { ... });
loop.onTimeout(time, [](React::Timer *timer) { ... });
loop.onInterval(time, [](React::Interval *interval) { ... });
loop.onSignal(signum, [](React::Signal *signal) { ... });
````

The objects that are passed to your callback function all have in common that
they have a cancel() method that you can call to stop further events from
being received. Next to the cancel() method, additional methods are available
to deal with the specific behavior of the item being watched.


RETURN VALUE OF LOOP METHODS
============================

The objects that are passed to the callback functions (React::Reader, 
React::writer, etc) are also returned from the event-registering functions.
Therefore, if you call Loop::onReadable() to register a function that should be
called when a filedescriptor becomes readable, the Loop::onReadable() method
immediately returns a reference to the React::Reader object that takes care
of


Although we have up to now ignored the return value of the various 
Loop::onSomething() methods, it can be very convenient to 

All Loop::onSomething() methods have a return value: the methods return a 
std::shared_ptr to the objects that are also passed to the callback
functions. We can use this to modify the first example. In that first example 
we set a timer to exit the program after five seconds. Now we are going to 
reset that timer every time that some input is detected. Only when no input
was received for a full five seconds, the program will terminate.

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

    // set a timer to stop the application if it is idle for five seconds
    // note that the type of 'timer' is std::shared_ptr<React::Timer>
    auto timer = loop.onTimeout(5.0, []() {
    
        // report that the timer expired
        std::cout << "timer expired" << std::endl;
    
        // stop the application
        exit(0);
    });
    
    // we'd like to be notified when input is available on stdin
    // the type of 'reader' is std::shared_ptr<React::Reader>
    auto reader = loop.onReadable(STDIN_FILENO, [timer]() {
    
        // read input
        std::string buffer;
        std::cin >> buffer;
    
        // show what we read
        std::cout << buffer << std::endl;
        
        // set the timer back to five seconds
        timer->set(5.0);
    });

    // handler when control+c is pressed
    loop.onSignal(SIGINT, [&loop, timer, reader]() {
        
        // report that we got a signal
        std::cout << "control+c detected" << std::endl;
        
        // both the timer, and the input checker can be cancelled now
        timer->cancel();
        reader->cancel();
        
        // stop the application in one second
        loop.onTimeout(1.0, [](Timer *timer) {
        
            // exit the application
            exit(0);
        });
    });

    // run the event loop
    loop.run();

    // done
    return 0;
}
````
