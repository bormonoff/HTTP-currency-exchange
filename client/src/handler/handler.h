#pragma once

#include <iostream>
#include <optional>

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>

#include "model/broker.h"

namespace network {

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

using tcp = net::ip::tcp;
using Request = http::request<http::string_body>;
using Response = http::response<http::string_body>; 

using namespace std::literals;

const std::string SERVER_IP = "127.0.0.1"s;
const size_t SERVER_PORT = 8080;

class Network {
public:
    Network();

    Network(const Network&) = delete;
    Network& operator=(const Network&) = delete;
    
    template<typename Handler>
    bool HandleRequest(Request&& req, Handler&& handler) {
        try {
            SendRequest(std::move(req));

            beast::flat_buffer buffer;
            auto response = ReadResponse(buffer);
            if (response) {
                ViewResponse(std::move(*response), std::move(handler));
            }
        } catch (std::exception& ex) {
            std::cout << ex.what() << std::endl;
        }
        return true;
    }

private:
    void SendRequest(Request&& req);

    template<typename Handler>
    void ViewResponse(Response&& res, Handler&& handler) {
        try {
            handler(std::move(res));
        } catch(std::exception& ex) {
            std::cout << "Server response is corruped" << std::endl;
            return;
        }
    }

    std::optional<Response> ReadResponse(beast::flat_buffer& buffer);

    net::io_context ioc_; 
    tcp::socket socket_;
    std::string token_;
};

}  // namespace network
