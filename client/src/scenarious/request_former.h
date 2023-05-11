#pragma once

#include <optional>

#include "model/broker.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp> 
#include <boost/json.hpp>

namespace json = boost::json;
namespace http = boost::beast::http;

using namespace std::literals;

using Request = http::request<http::string_body>;
using Response = http::response<http::string_body>; 

namespace scenario {

std::optional<Request> FormCreateOrderReq(const model::Broker& broker);
std::optional<Request> FormAddBrokerReq(const model::Broker& broker);

Request FormGetBrokerBalanceReq(const model::Broker& broker);
Request FormOrdersReq(const model::Broker& broker);
Request FormCompleteOrdersReq(const model::Broker& broker);
}  // namespace scenario
