#include <map>
#include <string>

namespace model {

struct BrokerBet {
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
    void AddCurrentBet(std::string& token, double count, double price, bool is_sell) {
        current_bets_.try_emplace(token, BrokerBet{count, price, is_sell});
    }
    const std::map<std::string, BrokerBet>& GetBrokerBets() { return current_bets_; }
    
private:
    double rub_{0.0};
    double usd_{0.0};
    std::string token_;
    std::map<std::string, BrokerBet> current_bets_;
};

}  // namespace model
