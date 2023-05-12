#include <boost/json.hpp>

#include "core/server.h"
#include "util/literal_storage.h"

namespace handler {

namespace http = boost::beast::http;
namespace json = boost::json;

using Response = http::response<http::string_body>;
using Request = http::request<http::string_body>;

Response InvalidBet(const Request& req);
}  // namespace handler
