#include <string>

namespace handler {
using namespace std::literals;

const std::string CONTENT_TYPE_JSON = "application/json"s;
const std::string NO_CACHE = "no-cache"s;

const std::string CODE = "Code"s;
const std::string MESSAGE = "Bet has been already created"s;
const std::string INVALID_BET_CODE = "invalidBet"s;
const std::string BET_HAS_ALREADY_CREATED = "Bet has been already created"s;
}  // namespace handler

namespace literals {
using namespace std::literals;

const std::string IS_SELL = "is_sell"s;
const std::string COUNT = "count"s;
const std::string PRICE = "price"s;
const std::string TOKEN = "token"s;
const std::string USD = "USD"s;
const std::string RUB = "RUB"s;
}  // namespace literals