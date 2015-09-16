/**
 *  socket_un.cpp
 *
 *  Unix Domain socket related tests
 *
 *  @copyright 2014 Copernica BV
 */

#include <thread>
#include <chrono>
#include <../reactcpp.h>
#include <gtest/gtest.h>

TEST(UnixDomainSocket, ClientTest)
{
    const char PATH[] = "/tmp/sock.reactcpp.test";
    const char SERVER_TX_MSG[] = "SERVER HELLO";
    const char CLIENT_TX_MSG[] = "CLIENT HELLO";

    using React::Tcp::Server;
    using React::Tcp::Connection;

    char server_rx_msg[64] = {0};
    char client_rx_msg[64] = {0};

    React::MainLoop loop;

    Server server(&loop, PATH);
    std::shared_ptr<Connection> conn1;

    server.onConnect([&]() -> bool {
        conn1 = std::shared_ptr<Connection>(new Connection(&server));

        conn1->onReadable([&]() -> bool {
            conn1->recv(server_rx_msg, sizeof(server_rx_msg));
            conn1->send(SERVER_TX_MSG, sizeof(SERVER_TX_MSG), 0);
            loop.stop();
            return false;
        });

        return false;
    });

    std::thread th([&]() {
        React::Loop inner;
        Connection client1(&inner, PATH);

        client1.onWritable([&]() -> bool {
            client1.send(CLIENT_TX_MSG, sizeof(CLIENT_TX_MSG), 0);
            return false;
        });

        client1.onReadable([&]() -> bool {
            client1.recv(client_rx_msg, sizeof(client_rx_msg));
            return false;
        });

        inner.run();
    });

    loop.run();
    th.join();

    ASSERT_STREQ(CLIENT_TX_MSG, server_rx_msg);
    ASSERT_STREQ(SERVER_TX_MSG, client_rx_msg);
}

