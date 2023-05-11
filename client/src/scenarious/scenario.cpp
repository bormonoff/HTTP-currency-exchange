#include "scenarious/scenario.h"

namespace scenario {

void Scenario::AddBroker() {
    if (auto it = FormAddBrokerReq(broker_)) {
        network_.HandleRequest(std::move(*it), [this](Response&& res){
            json::value json = json::parse(res.body());
            broker_.SetToken(static_cast<std::string>(json.at("token"s).as_string()));
        });
    } else {
        output_ << "Broker has already been authorized" << std::endl;
    }
}

bool Scenario::GetBalance() {
    network_.HandleRequest(FormGetBrokerBalanceReq(broker_), [this](Response&& res) {
        json::value json = json::parse(res.body());
        output_ << "Your current balance is: " << json.at("USD"s).as_double() << " USD and "
                << json.at("RUB"s).as_double() << " RUB." << std::endl;
    });
    return 1;
}

bool Scenario::GetOrders() {
    network_.HandleRequest(FormOrdersReq(broker_), [this](Response&& res) {
        json::value json = json::parse(res.body());
        output_ << "Your current balance is: " << json.at("USD"s).as_double() << " USD and "
                << json.at("RUB"s).as_double() << " RUB." << std::endl;
    });
    return 1;
}

bool Scenario::GetCompleteOrders() {
    network_.HandleRequest(FormCompleteOrdersReq(broker_), [this](Response&& res) {
        json::value json = json::parse(res.body());
        output_ << "Your current balance is: " << json.at("USD"s).as_double() << " USD and "
                << json.at("RUB"s).as_double() << " RUB." << std::endl;
    });
    return 1;
}

bool Scenario::CreateOrder() {
    if (auto it = FormCreateOrderReq(broker_)) {
        network_.HandleRequest(std::move(*it), [this](Response&& res){
            if (res.result() != http::status::ok) {
                ReadErrorReq(std::move(res));
                return 1;
            }
            
            try {
                json::value json = json::parse(res.body());
                bool is_sell = json.at("is_sell"s).as_bool();
                double count = json.at("count"s).as_double();
                if (is_sell) {
                    output_ << "You put on sale "s << count << " USD"s << std::endl;
                } else {
                    output_ << "You created an order to buy "s << count << " USD"s << std::endl;
                }
            } catch (std::exception& ex) {
                output_ << "Can't parse server request, ex: " << ex.what() << std::endl; 
            }
            return 1;
        });
    }
    return true;
}

void Scenario::ReadErrorReq(Response&& res) {
    json::value json = json::parse(res.body());
    output_ << static_cast<std::string>(json.at("Message"s).as_string()) << std::endl;
}
}  // namespace scenario
