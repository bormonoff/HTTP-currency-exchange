#include "broker.h"

namespace model {

void Broker::AddCurrentBet(std::string& token, double count, double price, bool is_sell) {
    current_bets_.try_emplace(token, BrokerBet{count, price, is_sell});
}

void Broker::AddCompletedBet(double count, double price, bool is_sell) {
    completed_bets_.push_front(BrokerBet{count, price, is_sell});
}

void Broker::UpdateCurrentBet(const std::string& token, double count, double price) {
    auto it = current_bets_.find(token);
    if (it != current_bets_.end()) {
        if ((*it).second.count <= count) {
            current_bets_.erase(it);
        } else {
            (*it).second.count -= count;
        }
    }
}

}  // namespace model
