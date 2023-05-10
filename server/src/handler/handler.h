#pragma once

#include <iostream>

#include "handler/scenarios.h"

namespace handler {
    
namespace beast = boost::beast;
namespace http = beast::http;

class RequestHandler {
public:
    explicit RequestHandler() {}

    RequestHandler(const RequestHandler&)   = delete;
    RequestHandler& operator=(const RequestHandler&) = delete;

    template <typename Body, typename Allocator, typename Send>
    void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {
        send(std::move(scenraios_.MakeResponse(req)));
    }        

private:
    Scenarios scenraios_;
};
}  // namespace handler
