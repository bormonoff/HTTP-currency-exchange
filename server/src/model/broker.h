#include <map>
#include <string>

namespace model {

struct BrokerBets {
    double count;
    std::string currency;
};

class Broker {
public:
    Broker(std::string token) 
        : token_{std::move(token)} {
    }

    Broker(const Broker&) = delete;
    Broker& operator=(const Broker&) = delete;

    const std::string& GetToken() const { return token_; }
    void AddMoney(double count) { balance += count; }

private:
    double balance;
    std::string token_;
    std::map<std::string, BrokerBets> current_bets_;
};

}  // namespace model
