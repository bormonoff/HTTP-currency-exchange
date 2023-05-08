#include <iostream>
#include <optional>
#include <thread>
#include <string_view>

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp> 

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using StringRequest = http::request<http::string_body>;
using StringResponse = http::response<http::string_body>; 
using tcp = net::ip::tcp;

std::optional<StringResponse> ReadResponse(tcp::socket& socket, beast::flat_buffer& buffer) {
    beast::error_code ec;
    StringResponse res;

    http::read(socket, buffer, res, ec);

    if (ec == http::error::end_of_stream) {
        return std::nullopt;
    }
    if(ec) {
        throw std::runtime_error("Failed to read response: "s.append(ec.message()));
    }
    return res;
}

template<typename T>
void ViewResponse(const T& response) {
    std::cout << response.body() << std::endl;
}

void Connect(tcp::socket& socket) {
 try {
    boost::beast::http::request<boost::beast::http::string_body> req(
    boost::beast::http::verb::post, "/", 11);

    req.body() = "Test client";
    req.content_length(req.body().size());

    write(socket, req);

    beast::flat_buffer buffer;

    auto response = ReadResponse(socket, buffer);
    ViewResponse(*response);

    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}

int main() {
    net::io_context ioc; 
    tcp::socket socket(ioc);
    boost::system::error_code ex;
    socket.connect(tcp::endpoint(net::ip::address::from_string("127.0.0.1"), 8080), ex);

    Connect(socket);    
}