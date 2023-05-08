#include "sdk.h"

#include <exception>
#include <thread>

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include <core/server.h>
#include <handler/handler.h>

namespace {

template <typename Fn>
void RunWorkers(unsigned max_threads, const Fn& fn) {
    max_threads = std::max(1u, max_threads);
    std::vector<std::jthread> workers;
    workers.reserve(max_threads - 1);
    while (--max_threads) {
        workers.emplace_back(fn);
    }
    fn();
}
}  // namespace

namespace net = boost::asio;
namespace beast = boost::beast;
namespace sys = boost::system;
namespace http = beast::http;

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
    try {
        const unsigned num_threads = std::thread::hardware_concurrency();
        net::io_context ioc(num_threads);

        const std::string address = "0.0.0.0";
        constexpr unsigned short port = 8080;
        const net::ip::tcp::endpoint endpoint{net::ip::make_address(address), port};

        handler::RequestHandler handler;

        http_server::ServeHttp(ioc, endpoint, [&handler](auto&& req, auto&& send) {
            handler(std::forward<decltype(req)>(req), std::forward<decltype(send)>(send));
        });

        std::cout << "Server has started..." << std::endl;

        RunWorkers(num_threads, [&ioc]{
            ioc.run();
        });

        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        return EXIT_FAILURE;
    }
}