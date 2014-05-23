/**
 *  Worker.cpp
 *
 *  Worker related tests
 *
 *  @copyright 2014 Copernica BV
 */

#include <../reactcpp.h>
#include <gtest/gtest.h>

TEST(Worker, StopFromMainWorker)
{
    React::MainLoop loop;
    // Timeout after 5 seconds
    loop.onTimeout(5.0, [&loop]() {
        loop.stop();
        FAIL() << "Timeout";
    });

    React::Worker worker(&loop);
    worker.execute([&loop]() {
        loop.stop();
    });

    loop.run();
}

TEST(Worker, StopFromInnerWorker)
{
    React::MainLoop loop;
    // Timeout after 5 seconds
    loop.onTimeout(5.0, [&loop]() {
        loop.stop();
        FAIL() << "Timeout";
    });

    React::Worker worker1;
    React::Worker worker2(&loop);
    worker1.execute([&loop,&worker2]() {
        worker2.execute([&loop]() {
            loop.stop();
        });
    });

    loop.run();
}