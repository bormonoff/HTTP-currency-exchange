#pragma once

#include "menu/menu.h"
#include "handler/handler.h"
#include "scenarious/request_former.h"

namespace scenario {

namespace ph = std::placeholders;

class Scenario {
public:
    explicit Scenario(menu::Menu& menu, network::Network& network, std::istream& input, std::ostream& output)
        : menu_{menu}
        , network_{network}
        , input_{input}
        , output_{output} {
        AddBroker();
        InitActios();
    }

    Scenario(const Scenario&) = delete;
    Scenario& operator=(const Scenario&) = delete;
    
private:
    void InitActios() {
        menu_.AddAction("NewOrder"s, "Create new order"s, 
                std::bind(&Scenario::CreateOrder, this));
        menu_.AddAction("GetBalance"s, "Get broker balance"s, 
                std::bind(&Scenario::GetBalance, this));
        menu_.AddAction("GetCurrentOrders"s, "Get all current orders"s, 
                std::bind(&Scenario::GetOrders, this));
        menu_.AddAction("GetCompleteOrders"s, "Get all completed orders"s, 
                std::bind(&Scenario::GetCompleteOrders, this));
    }

    void AddBroker();
    bool CreateOrder();
    bool GetBalance();
    bool GetOrders();
    bool GetCompleteOrders();

    void ReadErrorReq(Response&& res);

    model::Broker broker_;

    menu::Menu& menu_;
    network::Network& network_;
    std::istream& input_;
    std::ostream& output_;
};
}  // namespace scenario
