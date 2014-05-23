/**
 *  Read.cpp
 *
 *  Read related tests
 *
 *  @copyright 2014 Copernica BV
 */

#include <../reactcpp.h>
#include <gtest/gtest.h>

TEST(Read, CancelCallback)
{
    int fds[2];
    ASSERT_EQ(pipe(fds), 0); // Create a pipe
    bool done = false;
    React::MainLoop loop;
    // Timeout after 5 seconds
    loop.onTimeout(5.0, [&loop,&fds]() {
        loop.stop();
        close(fds[0]);
        close(fds[1]); // Close our pipe
        FAIL() << "Timeout";
    });
    char buf[] = "Hello world";
    ASSERT_EQ(write(fds[1], buf, sizeof(buf)), sizeof(buf)) << "Failed to write into our pipe!";
    loop.onReadable(fds[0], [&loop, &done]() {
        done = true;
        return false;
    });

    while (done == false) loop.step();

    close(fds[0]);
    close(fds[1]); // Close our pipe
}