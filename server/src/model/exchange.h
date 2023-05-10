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

    Broker& AddBroker() {
        std::string new_token = util::GenerateToken(32);
        return (*brokers_.try_emplace(new_token, new_token).first).second;
    }

private:
    std::map<std::string, Broker> brokers_;
};

class USDExchange : public Exchange {
public:
    const std::list<Bet> GetBuyOffers() const noexcept { return buy_offers_; }
    const std::list<Bet> GetSellOffers() const noexcept { return sell_offers_; }
    
    void AddBuyBet(Broker& broker, double price, double count) {
        for (auto it = sell_offers_.begin(); it != sell_offers_.end();) {
            if (it -> price > price) { break; }
            if (it -> count > count) {
                it -> broker.AddRUB(price * count);
                it -> broker.AddUSD(-(count));
                broker.AddRUB(-(price * count));
                broker.AddUSD(count);   

                it -> count -= count;
                count = 0;
                break;
            } else {
                it -> broker.AddRUB(price * it -> count);
                it -> broker.AddUSD(-(it -> count));
                broker.AddRUB(-(price * it -> count));
                broker.AddUSD(it -> count);  

                count -= it -> count;             
                ++it;
                sell_offers_.pop_front();
                continue;
            }
        }

        if (count) {
            RemoveBet(broker.GetToken(), buy_offers_);
            auto it = buy_offers_.begin();
            for (; it != buy_offers_.end(); ++it) {
                if (it -> price <= price) {
                    if (it -> price == price) {
                        ++it;
                    }
                    buy_offers_.insert(it, std::move(NotifyBrokerViaNewBet(price, count, broker, false)));
                    return;
                }
            }
            if (it == buy_offers_.end()) { 
                buy_offers_.push_back(std::move(NotifyBrokerViaNewBet(price, count, broker, false))); 
            }
            if (buy_offers_.empty()) { 
                buy_offers_.push_front(std::move(NotifyBrokerViaNewBet(price, count, broker, false))); 
            };
        }
    }

    void AddSellBet(Broker& broker, double price, double count) {
        for (auto it = buy_offers_.begin(); it != buy_offers_.end();) {
            if (it -> price < price) { break; }
            if (it -> count > count) {  
                it -> broker.AddRUB(-(it -> price * count));
                it -> broker.AddUSD(count);
                broker.AddRUB(it -> price * count);
                broker.AddUSD(-count);  

                it -> count -= count;
                count = 0;
                break;
            } else {
                it -> broker.AddRUB(-(it -> price * it -> count));
                it -> broker.AddUSD(it -> count);
                broker.AddRUB(it -> price * it -> count);
                broker.AddUSD(-(it -> count));  

                count -= it -> count;
                ++it;
                buy_offers_.pop_front();
                continue;
            }
        }

        if (count) {
            RemoveBet(broker.GetToken(), sell_offers_);
            auto it = sell_offers_.begin();
            for (; it != sell_offers_.end(); ++it) {
                if (it -> price >= price) {
                    if (it ->price != price) {
                        (std::advance(it, -2));
                    } else {
                        ++it;
                    }
                    sell_offers_.insert(it, std::move(NotifyBrokerViaNewBet(price, count, broker, true)));
                    return;
                }
            }
            if (it == sell_offers_.end()) { 
                sell_offers_.push_back(std::move(NotifyBrokerViaNewBet(price, count, broker, true))); 
            }
            if (sell_offers_.empty()) { 
                sell_offers_.push_front(std::move(NotifyBrokerViaNewBet(price, count, broker, true)));
            };
        }
    }

private:
    template<typename Container>
    void RemoveBet(const std::string& token, Container& container) {
        for (auto it = container.begin(); it != container.end(); ++it) {
            if (it -> broker.GetToken() ==  token) {
                it = container.erase(it);
                return;
            }
        }
    }

    Bet NotifyBrokerViaNewBet(double price, double count, Broker& broker, bool is_sell) {
        std::string bet_token{util::GenerateToken(16)};
        broker.AddCurrentBet(bet_token, count, price, is_sell);
        return {price, count, broker, std::move(bet_token)};
    }
    
    std::list<Bet> buy_offers_;
    std::list<Bet> sell_offers_;
};
}  // namespace model