#include "scenarios.h"

#include <boost/json/src.hpp>

namespace handler {

using namespace std::literals;

Response Scenarios::AddBroker(const Request& req) {
    auto& broker = ex_usd_.AddBroker();

    Response response{http::status::ok, req.version()};
    response.set(http::field::content_type, "application/json");
    response.set(http::field::cache_control, "no-cache");

    json::object json_response;
    json_response["token"] = broker.GetToken();
    response.body() = json::serialize(json_response);
    response.content_length(response.body().size());
    return response;
};
}  // namespace handler
