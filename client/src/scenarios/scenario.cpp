#include "scenarios/scenario.h"

#include "util/literal_storage.h"

namespace scenario {

void Scenario::AddBroker() {
    if (auto it = FormAddBrokerReq(broker_)) {
        network_.HandleRequest(std::move(*it), [this](Response&& res){
            json::value json = json::parse(res.body());
            broker_.SetToken(static_cast<std::string>(json.at(literals::TOKEN).as_string()));
        });
    } else {
        output_ << "Broker has already been authorized"s << std::endl;
    }
}

bool Scenario::GetBalance() {
    network_.HandleRequest(FormGetBrokerBalanceReq(broker_), [this](Response&& res) {
        json::value json = json::parse(res.body());
        output_ << "Your current balance is: "s << json.at(literals::USD).as_double() 
                << " USD and "<< json.at(literals::RUB).as_double() << " RUB."s << std::endl;
    });
    return 1;
}

bool Scenario::GetOrders() {
    network_.HandleRequest(FormOrdersReq(broker_), [this](Response&& res) {
        json::object json = json::parse(res.body()).get_object();

        if (!json.size()) {
            output_ << "You haven't current orders" << std::endl;
            return 1;
        }

        try {
            for (int i = 0; i < json.size(); ++i) {
                auto order = json.at(std::to_string(i)).as_array();
                double count =  order[0].as_double(); 
                double price = order[1].as_double();      
                bool is_sell =  order[2].as_bool();  
                output_ << "Order number " << i;
                if (is_sell) {
                    output_ << ": You want to sell "s; 
                } else {
                    output_ << ": You want to buy "s;
                }            
                output_ << count << " USD for "s << price << " RUB each. "s << std::endl; 
            }
        } catch (std::exception& ex) {
            output_ << "Can't parse server request, ex: "s << ex.what() << std::endl; 
        }
        return 1;
    });
    return 1;
}

bool Scenario::GetCompleteOrders() {
    network_.HandleRequest(FormCompleteOrdersReq(broker_), [this](Response&& res) {
        json::object json = json::parse(res.body()).get_object();

        if (!json.size()) {
            output_ << "You haven't current orders"s << std::endl;
            return 1;
        }

        try {
            for (int i = 0; i < json.size(); ++i) {
                auto order = json.at(std::to_string(i)).as_array();
                double count =  order[0].as_double(); 
                double price = order[1].as_double();      
                bool is_sell =  order[2].as_bool();  
                output_ << "Order number " << i;
                if (is_sell) {
                    output_ << ": You have sold "s; 
                } else {
                    output_ << ": You have bought "s;
                }            
                output_ << count << " USD for "s << price << " RUB each. "s << std::endl; 
            }
        } catch (std::exception& ex) {
            output_ << "Can't parse server request, ex: "s << ex.what() << std::endl; 
        }
        return 1;
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
                bool is_sell = json.at(literals::IS_SELL).as_bool();
                double count = json.at(literals::COUNT).as_double();
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
    output_ << static_cast<std::string>(json.at(literals::MESSAGE).as_string()) << std::endl;
}
}  // namespace scenario
