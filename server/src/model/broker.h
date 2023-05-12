#include <deque>
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
    using Orders = std::map<std::string, BrokerBet>;
    using CompleteOrders =  std::deque<BrokerBet>;

    Broker(std::string token) 
        : token_{std::move(token)} {
    }

    Broker(const Broker&) = delete;
    Broker& operator=(const Broker&) = delete;

    Broker(Broker&&) noexcept = default;
    Broker& operator=(Broker&&) noexcept = default;

    const std::string& GetToken() const { return token_; }
    double GetRoubles() const noexcept { return rub_; }
    double GetUSD() const noexcept { return usd_; }
    const Orders& GetBrokerBets() const noexcept { return current_bets_; }
    const CompleteOrders& GetCompleteBrokerBets() const noexcept { return completed_bets_; }

    void AddRUB(double count) { rub_ += count; }
    void AddUSD(double count) { usd_ += count; }
    void AddCurrentBet(std::string& token, double count, double price, bool is_sell);
    void RemoveCurrentBet(std::string& token);
    void AddCompletedBet(double count, double price, bool is_sell);
    void UpdateCurrentBet(const std::string& token, double count, double price);

    bool HasCurrentOrder();

private:
    double rub_{0.0};
    double usd_{0.0};
    std::string token_;
    Orders current_bets_;
    CompleteOrders completed_bets_;
};
}  // namespace model
