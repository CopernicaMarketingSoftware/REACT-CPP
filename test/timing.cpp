/**
 *  Timing.cpp
 *
 *  Timing related tests
 *
 *  @copyright 2014 Copernica BV
 */

#include <../reactcpp.h>
#include <gtest/gtest.h>

TEST(Timing, Interval)
{
    React::MainLoop loop;
    // Timeout after 5 seconds
    loop.onTimeout(5.0, [&loop]() {
        loop.stop();
        FAIL() << "Timeout";
    });

    bool done = false;
    int counter = 0;
    loop.onInterval(0.001, [&loop,&counter,&done]() {
        if (++counter == 5)
        {
            done = true;
            loop.stop();
            return false;
        }
        return true;
    });

    while (done == false) loop.step();

    EXPECT_EQ(5, counter);
}