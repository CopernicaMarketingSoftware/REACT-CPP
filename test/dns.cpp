/**
 *  Dns.cpp
 *
 *  Dns related tests
 *
 *  @copyright 2014 Copernica BV
 */

#include <../reactcpp.h>
#include <gtest/gtest.h>

/**
 *  This test will require a working network connection!
 */
TEST(DNS, ARequest)
{
    React::MainLoop loop;
    // Timeout after 5 seconds
    loop.onTimeout(5.0, [&loop]() {
        loop.stop();
        FAIL() << "Timeout";
    });
    React::Dns::Resolver resolver(&loop);

    resolver.ip("www.copernica.com", [&loop](React::Dns::IpResult &&ips, const char *error) {
        EXPECT_GE(ips.size(), 1);
        loop.stop();
    });

    loop.run();
}

TEST(DNS, MXRequest)
{
    React::MainLoop loop;
    // Timeout after 5 seconds
    loop.onTimeout(5.0, [&loop]() {
        loop.stop();
        FAIL() << "Timeout";
    });
    React::Dns::Resolver resolver(&loop);

    resolver.mx("copernica.com", [&loop](React::Dns::MxResult &&mxs, const char *error) {
        EXPECT_GE(mxs.size(), 1);
        loop.stop();
    });

    loop.run();
}