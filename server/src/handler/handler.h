#pragma once

#include <iostream>
#include <mutex>

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
        std::unique_lock<std::mutex> lock{mutex_};
        send(std::move(scenraios_.MakeResponse(req)));
    }        

private:
    std::mutex mutex_;
    Scenarios scenraios_;
};
}  // namespace handler
