/**
 *  Ip.cpp
 *
 *  Ip related tests
 *
 *  @copyright 2014 Copernica BV
 */

#include <../reactcpp.h>
#include <gtest/gtest.h>

TEST(IP, IPv4)
{
    React::Net::Ipv4 ip("192.168.1.0");
    EXPECT_TRUE(ip.valid());
    EXPECT_EQ("192.168.1.0", ip.toString());
    React::Net::Ipv4 invalid;
    EXPECT_FALSE(invalid.valid());
}

TEST(IP, IPv6)
{
    React::Net::Ipv6 ip("fe80::d6be:d9ff:fe6c:a91e");
    EXPECT_TRUE(ip.valid());
    EXPECT_EQ("fe80::d6be:d9ff:fe6c:a91e", ip.toString());
    React::Net::Ipv6 invalid;
    EXPECT_FALSE(invalid.valid());
}