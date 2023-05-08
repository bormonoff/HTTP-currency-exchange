#include "sdk.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

int main() {
    try {
        const unsigned num_threads = std::thread::hardware_concurrency();
        net::io_context ioc(num_threads);

        const std::string address = "0.0.0.0";
        constexpr unsigned short port = 8080;
        const net::ip::tcp::endpoint endpoint{net::ip::make_address(address), port};
        
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        return EXIT_FAILURE;
    }
}