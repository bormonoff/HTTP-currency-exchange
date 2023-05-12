#include "scenarios.h"

#include <boost/json/src.hpp>

namespace handler {

Response Scenarios::AddBroker(const Request& req) {
    auto& broker = ex_usd_.AddBroker();

    Response response{http::status::ok, req.version()};
    response.set(http::field::content_type, CONTENT_TYPE_JSON);
    response.set(http::field::cache_control, NO_CACHE);

    json::object json_response;
    json_response[literals::TOKEN] = broker.GetToken();

    response.body() = json::serialize(json_response);
    response.content_length(response.body().size());
    return response;
};

Response Scenarios::GetOrders(const Request& req) {
    Response response{http::status::ok, req.version()};
    response.set(http::field::content_type, CONTENT_TYPE_JSON);
    response.set(http::field::cache_control, NO_CACHE);

    std::string token = static_cast<std::string>(req[http::field::authorization]);
    auto& broker = ex_usd_.FindBroker(token);

    json::object json_response;
    for (auto& [bet_token, order] : broker.GetBrokerBets()) {
        json::array bet;
        size_t id{0};
        bet.push_back(order.count);
        bet.push_back(order.price);
        bet.push_back(order.is_sell);     
        json_response[std::to_string(id++)] = bet;   
    }

    response.body() = json::serialize(json_response);
    response.content_length(response.body().size());
    return response;
};

Response Scenarios::GetCompleteOrders(const Request& req) {
    Response response{http::status::ok, req.version()};
    response.set(http::field::content_type, CONTENT_TYPE_JSON);
    response.set(http::field::cache_control, NO_CACHE);

    std::string token = static_cast<std::string>(req[http::field::authorization]);
    auto& broker = ex_usd_.FindBroker(token);

    json::object json_response;
    size_t id{0};
    for (auto& it : broker.GetCompleteBrokerBets()) {
        json::array bet;
        bet.push_back(it.count);
        bet.push_back(it.price);
        bet.push_back(it.is_sell);     
        json_response[std::to_string(id++)] = bet;   
    }
    
    response.body() = json::serialize(json_response);
    response.content_length(response.body().size());
    return response;
};

Response Scenarios::GetBalance(const Request& req) {
    Response response{http::status::ok, req.version()};
    response.set(http::field::content_type, CONTENT_TYPE_JSON);
    response.set(http::field::cache_control, NO_CACHE);

    std::string token = static_cast<std::string>(req[http::field::authorization]);
    auto& broker = ex_usd_.FindBroker(token);

    json::object json_response;
    json_response[literals::USD] = broker.GetUSD();
    json_response[literals::RUB] = broker.GetRoubles();   

    response.body() = json::serialize(json_response);
    response.content_length(response.body().size());
    return response;
};

Response Scenarios::CreateOrder(const Request& req) {
    Response response{http::status::ok, req.version()};
    response.set(http::field::content_type, CONTENT_TYPE_JSON);
    response.set(http::field::cache_control, NO_CACHE);

    std::string token = static_cast<std::string>(req[http::field::authorization]);
    auto& broker = ex_usd_.FindBroker(token);

    double count{}, price{};
    bool is_sell{};
    try {
        json::value parse_request = json::parse(req.body());
        is_sell = parse_request.at(literals::IS_SELL).as_bool();
        count = parse_request.at(literals::COUNT).as_double();
        price = parse_request.at(literals::PRICE).as_double();
    } catch(std::exception& ex) {
        throw std::runtime_error("Request can't be parsed");
    }

    if (broker.HasCurrentOrder()) {
        return InvalidBet(req);
    }

    if (is_sell) {
        ex_usd_.AddSellBet(broker, price, count);
    } else {
        ex_usd_.AddBuyBet(broker, price, count);
    }

    json::object body;
    body[literals::IS_SELL] = is_sell;
    body[literals::COUNT] = count;

    response.body() = json::serialize(body);
    response.content_length(response.body().size());
    return response;
};
}  // namespace handler
