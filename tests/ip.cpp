#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <reactcpp.h>
#include <reactcpp/net/ipv4.h>
#include <reactcpp/net/ipv6.h>
#include <reactcpp/net/ip.h>
#include <iostream>

int main()
{
    React::Net::Ipv4 ipv4("192.168.1.0");
    
    std::cout << ipv4.toString() << std::endl;
    
    return 0;

}
