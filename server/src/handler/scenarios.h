#include "model/exchange.h"
#include "handler/response_storage.h"

namespace handler {

class Scenarios {
public:
    Scenarios() {
        scenario_["/broker"] = {[this](const Request& req) {
            return AddBroker(req);
        }};
        scenario_["/broker/orders"] = {[this](const Request& req) {
            return GetOrders(req);
        }};
        scenario_["/broker/complete_orders"] = {[this](const Request& req) {
            return GetCompleteOrders(req);
        }};
        scenario_["/broker/balance"] = {[this](const Request& req) {
            return GetBalance(req);
        }};
        scenario_["/broker/neworder"] = {[this](const Request& req) {
            return CreateOrder(req);
        }};
    }

    Response MakeResponse(Request& req) {
        return (scenario_.find(static_cast<std::string>(req.target())) 
            -> second(req));
    }

private:
    model::USDExchange ex_usd_;
    std::map<std::string, std::function<Response(Request)>> scenario_;

    Response AddBroker(const Request& req);
    Response GetOrders(const Request& req);
    Response CreateOrder(const Request& req);
    Response GetCompleteOrders(const Request& req);
    Response GetBalance(const Request& req);
};
}  // namespace handler
