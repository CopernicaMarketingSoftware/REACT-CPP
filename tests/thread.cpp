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
    
    // install an onSynchronize callback that is called when a worker thread
    // is ready with its task, the returned synchronizer object is of type
    // std::shared_ptr<Synchronizer>, and contains a thread safe pointer that
    // can be access from the other thread to notify us
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
