/**
 *  Thread.cpp
 *
 *  Thread related tests
 *
 *  @copyright 2014 Copernica BV
 */

#include <../reactcpp.h>
#include <gtest/gtest.h>
#include <thread>

TEST(Thread, Synchronize)
{
    React::MainLoop loop;
    // Timeout after 5 seconds
    loop.onTimeout(5.0, [&loop]() {
        loop.stop();
        FAIL() << "Timeout";
    });

    auto synchronizer = loop.onSynchronize([&loop]() {
        loop.stop();
        return true;
    });

    std::thread thread([synchronizer]() {
        synchronizer->synchronize();
    });

    loop.run();

    thread.join();
}