#pragma once

#include <core/server.h>

#include <iostream>

namespace handler {
    
namespace beast = boost::beast;
namespace http = beast::http;

class RequestHandler {
public:
    explicit RequestHandler() {}

    RequestHandler(const RequestHandler&)   = delete;
    RequestHandler& operator=(const RequestHandler&) = delete;

    template <typename Body, typename Allocator, typename Send>
    void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {\
        http::response<http::string_body> response{http::status::not_found, req.version()};
        response.set(http::field::content_type, "text/plain");

        response.body() = "test";
        response.content_length(response.body().size());

        send(std::move(response));
    }                                 
};
}  // namespace handler