#include <list>

#include "model/broker.h"
#include "util/util.h"

namespace model {

struct Bet {
    double price;
    double count;
    Broker& broker;
    std::string bet_token;
};

class Exchange {
public:    
    Exchange() = default;

    Exchange(const Exchange&) = delete;
    Exchange& operator=(const Exchange&) = delete;

    Broker& AddBroker();
    Broker& FindBroker(std::string& token);

private:
    std::map<std::string, Broker> brokers_;
};

class USDExchange : public Exchange {
public:
    using Offers = std::list<Bet>;
    
    const Offers GetBuyOffers() const noexcept { return buy_offers_; }
    const Offers GetSellOffers() const noexcept { return sell_offers_; }
    
    void AddBuyBet(Broker& broker, double price, double count);
    void AddSellBet(Broker& broker, double price, double count);

private:
    template<typename Container>
    void RemoveBet(const std::string& token, Container& container) {
        for (auto it = container.begin(); it != container.end(); ++it) {
            if (it -> broker.GetToken() ==  token) {
                it -> broker.RemoveCurrentBet(it -> bet_token);
                it = container.erase(it);
                return;
            }
        }
    }

    Bet NotifyBrokerViaNewBet(double price, double count, Broker& broker, 
                              bool is_sell);
    void UpdateBrokerAccount(Broker& broker, double price, double count, 
                             bool is_sell, const std::string& token = "");

    Offers buy_offers_;
    Offers sell_offers_;
};
}  // namespace model