#include "model/exchange.h"

#include <boost/json.hpp>

#include "core/server.h"

namespace handler {

namespace http = boost::beast::http;
namespace json = boost::json;


using Response = http::response<http::string_body>;
using Request = http::request<http::string_body>;

class Scenarios {
public:
    Scenarios() {
        scenario_["/broker"] = {[this](const Request& req) {
            return AddBroker(req);
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
};
}  // namespace handler
