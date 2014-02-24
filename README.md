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
    loop.onReadable(STDIN_FILENO, [](React::ReadWatcher *reader) {
    
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
React::ReadWatcher object, to inform the event loop that it is no longer 
interested in read events.

In the above example, this also means that the program automatically exits after
the first line has been read. The reason for this is that the run() method of 
the React::Loop and React::MainLoop classes automatically stops running when 
there are no more callback functions active. By calling the ReadWatcher::cancel()
method, the last and only registered callback function is cancelled, and the
event loop has nothing left to monitor.

The type of the parameter that is passed to your callback function depends on
the type of callback function that you have registered.

````c++
loop.onReadable(fd, [](React::ReadWatcher *watcher) { ... });
loop.onWritable(fd, [](React::WriteWatcher *watcher) { ... });
loop.onTimeout(time, [](React::TimeoutWatcher *watcher) { ... });
loop.onInterval(time, [](React::IntervalWatcher *watcher) { ... });
loop.onSignal(signum, [](React::SignalWatcher *watcher) { ... });
````

The objects that are passed to your callback function all have in common that
they have a cancel() method that you can call to stop further events from
being received. Next to the cancel() method, additional methods are available
to deal with the specific behavior of the item being watched.


RETURN VALUE OF LOOP METHODS
============================

The objects that are passed to the callback functions (React::ReadWatcher, 
React::WriteWatcher, etc) are also returned from the event-registering functions.
This means that it is possible to store the return value of a call to 
Loop::onReadable() in a variable, and that you will not have to wait for the
callback to be called to cancel further calls to it.

All Loop::onSomething() methods return a shared_ptr to the watcher object. 
You may store this shared_ptr if you'd like to use it in the future, but you do 
not have to. Internally, the library
also keeps a pointer to the object, and will pass on that pointer to your callback 
every time it is called. So even if you decide to discard the return value, the 
object will live on. The only way to stop the callback from being active is by 
calling the cancel() method on the returned object, or on the same object
inside your callback function.

With this knowledge we are going to modify our earlier example. The echo 
application that we showed before is updated to set the timer back to five
seconds every time that some input is read, so that the application will no
longer stop five seconds after it was started, but five seconds after the last
input was received. We also change the signal watcher: the moment CTRL+C is 
pressed, the application will stop responding, but it will delay it's exit 
for one second.

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
    // note that the type of 'timer' is std::shared_ptr<React::TimeoutWatcher>
    auto timer = loop.onTimeout(5.0, []() {
    
        // report that the timer expired
        std::cout << "timer expired" << std::endl;
    
        // stop the application
        exit(0);
    });
    
    // we'd like to be notified when input is available on stdin
    // the type of 'reader' is std::shared_ptr<React::ReadWatcher>
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
        loop.onTimeout(1.0, []() {
        
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

CONSTRUCT WATCHER OBJECTS
=========================

Up to now we have registered callback methods via the Loop::onSomething()
methods. These methods return a shared pointer to an object that keeps the
watcher state. It is also possible to create such objects directly, without 
calling a Loop::onSomething method(). This can be very convenient, because
you will have ownership of the object (instead of the event loop) and you can 
unregister your handler function by just destructing the object.

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
    React::ReadWatcher reader(loop, STDIN_FILENO, []() {
    
        // read input
        std::string buffer;
        std::cin >> buffer;
    
        // show what we read
        std::cout << buffer << std::endl;
    });

    // run the event loop
    loop.run();

    // done
    return 0;
}
````

Conceptually, there is not a big difference between calling Loop::onReadable()
to register a callback function, or by instantiating a React::ReadWatcher object
yourself. In my opinition, the code that utilizes a call to Loop::onReadable() 
is easier to understand and maintain, but by creating a ReadWatcher class yourself,
you have full ownership of the class and can destruct it whenever you like -
which can be useful too.


FILEDESCRIPTORS
===============

Filedescriptors can be checked for activity by registering callbacks for 
readability and writability. The loop object has two methods for that:

````c++
std::shared_ptr<ReadWatcher> Loop::onReadable(int fd, const ReadCallback &callback);
std::shared_ptr<WriteWatcher> Loop::onWritable(int fd, const WriteCallback &callback);
````

Two possible callback signatures are accepted, one that takes a pointer to a
watcher object (React::ReadWatcher* for the callback to onReadable(), and 
React::WriteWatcher* for the callback to onWritable()), and a callback that does not
accept parameters at all.

````c++
loop.onReadable(fd, [](ReadWatcher *watcher) { ... });
loop.onReadable(fd, []() { ... });
loop.onWritable(fd, [](WriteWatcher *watcher) { ... });
loop.onWritable(fd, []() { ... });
````

You can also create a ReadWatcher or WriteWatcher object yourself. In that case you will
not have to use the Loop::onReadable() or Loop::onWritable() methods:

````c++
ReadWatcher watcher(&loop, fd, [](ReadWatcher *watcher) { ... });
ReadWatcher watcher(&loop, fd, []() { ... });
WriteWatcher watcher(&loop, fd, [](WriteWatcher *watcher) { ... });
WriteWatcher watcher(&loop, fd, []() { ... });
````

TIMERS AND INTERVALS
====================

The React library supports both intervals and timers. A timer is triggered
only once, an interval on the other hand calls the registered callback method 
every time the interval time has expired.

When you create an interval, you can specify both the initial expire time as
well as the interval between all subsequent calls. If you omit the initial time,
the callback will be first called after the first interval has passed.

````c++
std::shared_ptr<TimeoutWatcher> 
Loop::onTimeout(Timestamp seconds, const TimeoutCallback &callback);

std::shared_ptr<IntervalWatcher> 
Loop::onInterval(Timestamp interval, const IntervalCallback &callback);

std::shared_ptr<IntervalWatcher> 
Loop::onInterval(Timestamp initial, Timestamp interval, const IntervalCallback &callback);
````

Just like the callbacks for filedescriptors, the callbacks for timers and intervals 
also come in two forms: with and without a parameter.

````c++
loop.onTimeout(3.0, [](TimeoutWatcher *watcher) { ... });
loop.onTimeout(3.0, []() { ... });
loop.onInterval(5.0, [](IntervalWatcher *watcher) { ... });
loop.onInterval(5.0, []() { ... });
loop.onInterval(0.0, 5.0, [](IntervalWatcher *watcher) { ... });
loop.onInterval(0.0, 5.0, []() { ... });
````

And you can of course also instantiate React::TimeoutWatcher and 
React::IntervalWatcher objects directly:

````c++
TimeoutWatcher watcher(&loop, 3.0, [](TimeoutWatcher *watcher) { ... });
TimeoutWatcher watcher(&loop, 3.0, []() { ... });
IntervalWatcher watcher(&loop, 5.0, [](IntervalWatcher *watcher) { ... });
IntervalWatcher watcher(&loop, 5.0, []() { ... });
IntervalWatcher watcher(&loop, 2.0, 5.0, [](IntervalWatcher *watcher) { ... });
IntervalWatcher watcher(&loop, 2.0, 5.0, []() { ... });
````

SIGNALS
=======

Signals can be watched too. Normally, signals are delivered to your application
in an asynchronous way, and the signal handling code could be started when your
application is in the middle of running some other algorithm. By registering a 
signal handler via the React::MainLoop class, you can prevent this, and have 
your signal handling be executed as part of the event loop.

Setting up a signal handler is just as easy as setting up callback functions
for filedescriptors or timers. The loop object has a simple onSignal() method
for it:

````c++
std::shared_ptr<SignalWatcher> MainLoop::onSignal(int signum, const SignalCallback &callback);
````

And the callback function comes (of course) in two versions:

````c++
loop.onSignal(SIGTERM, [](SignalWatcher *watcher) { ... });
loop.onSignal(SIGTERM, []() { ... });
````

And for signals it is of course also possible to bypass the methods on the 
loop class, and create a React::Signal object yourself:

````c++
SignalWatcher watcher(&loop, SIGTERM, [](SignalWatcher *watcher) { ... });
SIgnalWatcher watcher(&loop, SIGTERM, []() { ... });
````

CHILD PROCESSES
===============

The MainLoop class also allows you to watch for status change events from child
processes. This is useful if your application forks off child processes, and 
wants to be notified when one of these child processes changes its status (like
exiting).

Both the pid and trace parameters are optional. If you do not specify a pid (or
set the parameter to zero), your callback function will be called for every
single child process that changes its status. The boolean trace parameter can
be used to indicate whether you'd like to be notified for every possible status
change (including changes between paused and running state), or only when child
processes terminate. Set trace to true to receive all notifications, and to false
to receive only for process exits.

````c++
std::shared_ptr<StatusWatcher> MainLoop::onStatusChange(pid_t pid, bool trace, const SignalCallback &callback);
````

And the callback function comes (of course) in two versions:

````c++
loop.onStatusChange(pid, false, [](StatusWatcher *watcher) { ... });
loop.onStatusChange(pid, false, []() { ... });
````

And just like all other watcher objects, you can also create StatusWatcher objects
yourself:

````c++
StatusWatcher watcher(&loop, pid, trace, [](StatusWatcher *watcher) { ... });
StatusWatcher watcher(&loop, pid, trace, []() { ... });
````

THREAD SYNCHRONIZATION
======================

Let's introduce a topic that has not been addressed in one of the
examples: running multiple threads and optionally multiple thread loops.

If your application runs multiple threads, there is a pretty good chance
that sooner or later you want to get these threads in sync. When you have, for
example, a worker thread that wants to report its results back to the
main thread, it should somehow notify that thread that the result of the
calculatations are somewhere to be picked up. If the main thread is busy running
an event loop, it should be able to interupt that event loop, so that the data 
can be picked up. This all can be done with the Loop::onSynchronize() method.

````c++
#include <reactcpp.h>
#include <unistd.h>
#include <iostream>
#include <thread>

/**
 *  Main procedure
 */
int main()
{
    // create a thread loop
    React::MainLoop loop;
    
    // install a callback that can be called by a worker thread.
    // the returned synchronizer object is of type std::shared_ptr<SynchronizeWatcher>, 
    // and contains a thread safe object that can be accessed from other threads 
    // to notify us
    auto synchronizer = loop.onSynchronize([]() {
        std::cout << "other thread has finished running" << std::endl;
    });
    
    // start a new thread
    std::thread thread([synchronizer]() {
    
        // some long running algorithm
        sleep(1);
        
        // notify the main event loop that the task was completed
        synchronizer->synchronize();
    });
    
    // we'd like to be notified when input is available on stdin
    loop.onReadable(STDIN_FILENO, []() {
    
        // read input
        std::string buffer;
        std::cin >> buffer;
    
        // show what we read
        std::cout << buffer << std::endl;
    });

    // run the event loop
    loop.run();
    
    // join the thread
    thread.join();
    
    // done
    return 0;
}
````

The example above demonstrates how threads can synchronize with each other.
First, you create an endpoint that the other thread can use to call the
main thread, and you install a handler that will be called when the
other thread uses that endpoint. Both steps are taken by a simple call to
Loop::onSynchronize(). This installs the callback function that runs in the 
main thread, and it returns the thread safe endpoint that can be used by other
thread to interup the main event loop.

The SynchronizeWatcher is similar to classes like React::ReadWatcher, React::WriteWatcher, 
React::TimeoutWatcher, etcetera. And the callback also comes in two forms: one with
a parameter and one without, and the watcher can be registered via a call
to Loop::onSynchronize() and by creating a Synchronizer object directly:

````c++
// example how to install a synchronizer via the Loop class
loop.onSynchronize([](SynchronizeWatcher *watcher) { ... });
loop.onSynchronize([]() { ... });

// example how to install the synchronizer as an object
SynchronizeWatcher watcher(loop, [](SynchronizeWatcher *watcher) { ... });
SynchronizeWatcher watcher(loop, []() { ... });
````

When you use this technology to synchronize threads, you probably need to have
some shared data. You could for example use a queue that is accessible by both
threads. The worker thread pushes results to it, then calls synchronizer->synchronnize()
to notify the master thread, which can then pick up the results from the queue.
If the queue object is not thread safe, you must make sure that you protect
access to it, for example by using mutex variables.

