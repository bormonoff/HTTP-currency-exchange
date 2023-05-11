#include "handler/handler.h"

namespace network {

Network::Network() 
    : socket_{ioc_} {
    boost::system::error_code ex;
    socket_.connect(tcp::endpoint(net::ip::address::from_string(SERVER_IP), 
                                  SERVER_PORT), ex);

    if(ex) {
        std::cout << "client can't connect to a server"s << std::endl;
    }
}

void Network::SendRequest(Request&& req) {
    boost::system::error_code ex;
    write(socket_, req, ex);
    if (ex) {
        std::cout << "message can't be send due to the network problems"s << std::endl;
    }
}

std::optional<Response> Network::ReadResponse(beast::flat_buffer& buffer) {
    beast::error_code ec;
    Response res;

    http::read(socket_, buffer, res, ec);

    if (ec == http::error::end_of_stream) {
        return std::nullopt;
    }
    if(ec) {
        std::cout << "message can't be read due to the network problems"s << std::endl;
    }
    return res;
}
}  // namespace network
