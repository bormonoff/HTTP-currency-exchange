#include <list>

#include "model/broker.h"
#include "util/util.h"

namespace model {

struct Bet {
    double price;
    double count;
    std::string bet_token;
    bool is_sell;
    Broker& broker;
};

class Exchange {
public:    
    Broker& AddBroker() {
        std::string new_token = util::GenerateToken();
        return (*brokers_.try_emplace(new_token, new_token).first).second;
    }

private:
    std::map<std::string, Broker> brokers_;
};

class USDExchange : public Exchange {
public:
    void AddBuyBet(Broker& broker, double price, double count) {
        auto it = bets_.rbegin();
        for(; it != bets_.rend(); ++it) {
            if (it -> broker.GetToken() == broker.GetToken()) { 
                it = decltype(it)(bets_.erase(std::next(it).base()));
                continue;
            }
            if (!(it -> is_sell) && it -> price >= price) {
                std::advance(it, -3);
                bets_.insert(it.base(), {price, count, util::GenerateToken(), false, broker});
                return;
            }
            if (it -> is_sell && it -> price <= price) {
                if (it -> count >= count) {
                    it -> count -= count;
                    break;
                } else {
                    count -= it -> count;
                    it = decltype(it)(bets_.erase(std::next(it).base()));
                }
            }
        }
        if (it == bets_.rend()) {
            bets_.insert(it.base(), {price, count, util::GenerateToken(), false, broker});
            return;
        }
    }

    void AddSellBet(Broker& broker, double price, double count) {
        auto it = bets_.begin();
        for (; it != bets_.end(); ++it) {
            if (it -> broker.GetToken() == broker.GetToken()) { 
                if (it -> price > price) { continue; }
                it = bets_.erase(it);
                continue;
            }
            if (it -> is_sell && it -> price >= price) {
                std::advance(it, -2);
                bets_.insert(it, {price, count, util::GenerateToken(), true, broker});
                return;
            }
            if (!(it -> is_sell) && it -> price <= price) {
                if (it -> count >= count) {
                    it -> count -= count;
                    break;
                } else {
                    count -= it -> count;
                    it = bets_.erase(it);
                    continue;
                }
            }
        }

        if (it == bets_.end()) {
            bets_.insert(it, {price, count, util::GenerateToken(), true, broker});
        }
    }

private:
    std::list<Bet> bets_;
};
}  // namespace model