#include <reactcpp.h>

using namespace React;

int main() {
    React::MainLoop loop;

    React::Curl::CurlMulti multi(&loop);

    React::Curl::Request request(&multi, "http://httpbin.org/headers");
    request.execute()
        .onSuccess([&multi](React::Curl::Result &&result) {
            std::cout << "Callback" << std::endl;
            std::cout << result.body() << std::endl;
            std::cout << "Headers" << std::endl;
            std::cout << result.headers() << std::endl;
            React::Curl::Request request(&multi, "http://httpbin.org/get");
            request.execute()
                .onSuccess([](React::Curl::Result &&result) {
                    std::cout << "Inner callback" << std::endl;
                    std::cout << result.body() << std::endl;
                });
        })
        .onFailure([](const char *error) {
            std::cout << "Error: " << error << std::endl;
        });

    loop.run();
    return 0;
};