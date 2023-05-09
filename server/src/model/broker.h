#include <map>
#include <string>

namespace model {

struct BrokerBets {
    double count;
    double price;
    bool is_sell;
};

class Broker {
public:
    Broker(std::string token) 
        : token_{std::move(token)} {
    }

    Broker(const Broker&) = delete;
    Broker& operator=(const Broker&) = delete;

    const std::string& GetToken() const { return token_; }
    double GetRoubles() const noexcept { return rub_; }
    double GetUSD() const noexcept { return usd_; }
    void AddRUB(double count) { rub_ += count; }
    void AddUSD(double count) { usd_ += count; }
    
private:
    double rub_{0.0};
    double usd_{0.0};
    std::string token_;
    std::map<std::string, BrokerBets> current_bets_;
};

}  // namespace model
