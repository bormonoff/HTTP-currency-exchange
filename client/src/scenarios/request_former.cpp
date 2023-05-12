#include "scenarios/request_former.h"

#include <iostream>

#include "util/util.h"
#include "util/literal_storage.h"

namespace scenario {

std::optional<Request> FormCreateOrderReq(const model::Broker& broker) {
    http::request<http::string_body> request(http::verb::post, "/broker/neworder", 11);
    request.set(http::field::authorization, broker.GetToken());

    std::cout << "Do you want to buy(1) or sell(2) USD?"s << std::endl;
    std::string str_choise;
    std::cin >> str_choise;
    if (!(str_choise == "1"s || str_choise == "2"s)) {
        std::cout  << "You try to make invalid choise. Return."s << std::endl;
        return std::nullopt;
    }
    bool is_sell = static_cast<bool>(str_choise[0] - '1');

    std::string str_count;
    if (!is_sell) {
        std::cout << "How many usd do you want to buy? (example: 30.3829)"s << std::endl;
        std::cin  >> str_count;
        if (!util::IsDouble(str_count)) {
            std::cout << "You try to buy not valid count. Return."s << std::endl;
            return std::nullopt;
        }
    } else {
        std::cout  << "How many usd do you want to sell? (example: 30.3829)"s << std::endl;
        std::cin >> str_count;
        std::cout.flush();
        if (!util::IsDouble(str_count)) {
            std::cout  << "You try to sell not valid count. Return."s << std::endl;
            return std::nullopt;
        }
    }

    std::cout << "Enter a price for one USD: "s << std::endl;
    std::string str_price;
    std::cin  >> str_price;
    std::cout.flush();
    if (!util::IsDouble(str_price)) {
        std::cout  << "You try to write not valid price. Return."s << std::endl; 
        return std::nullopt;    
    }
    
    json::object body;
    body[literals::IS_SELL] = is_sell;
    body[literals::COUNT] = std::atof(str_count.c_str());
    body[literals::PRICE] = std::atof(str_price.c_str());

    std::string a = json::serialize(body);
    request.body() = json::serialize(body);
    request.content_length(request.body().size());
    return request;
}

std::optional<Request> FormAddBrokerReq(const model::Broker& broker) {
    if (broker.GetToken() != "") {
        return std::nullopt;
    }
    http::request<http::string_body> request(http::verb::get, "/broker"s, 11);
    return request;
}

Request FormGetBrokerBalanceReq(const model::Broker& broker) {
    http::request<http::string_body> request(http::verb::get, "/broker/balance"s, 11);
    request.set(http::field::authorization, broker.GetToken());
    return request;
}

Request FormOrdersReq(const model::Broker& broker) {
    http::request<http::string_body> request(http::verb::get, "/broker/orders"s, 11);
    request.set(http::field::authorization, broker.GetToken());
    return request;
}

Request FormCompleteOrdersReq(const model::Broker& broker) {
    http::request<http::string_body> request(http::verb::get, "/broker/complete_orders"s, 11);
    request.set(http::field::authorization, broker.GetToken());
    return request;
}
}  // namespace scenario