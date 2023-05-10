#include "model/exchange.h"

namespace model {

void USDExchange::AddBuyBet(Broker& broker, double price, double count) {
    for (auto it = sell_offers_.begin(); it != sell_offers_.end();) {
        if (it -> price > price || count == 0) { break; }
        if (it -> count > count) {
            UpdateBrokerAccount(it -> broker, price, count, true, 
                                it -> bet_token);
            UpdateBrokerAccount(broker, price, count, false, it -> bet_token);

            it -> count -= count;
            count = 0;
            break;
        } else {
            UpdateBrokerAccount(it -> broker, price, it -> count, true, 
                                it -> bet_token);
            UpdateBrokerAccount(broker, price, it -> count, false, 
                                it -> bet_token);

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
                buy_offers_.insert(it, std::move(NotifyBrokerViaNewBet(price, 
                                                                       count, 
                                                                       broker, 
                                                                       false)));
                return;
            }
        }
        if (it == buy_offers_.end()) { 
            buy_offers_.push_back(std::move(NotifyBrokerViaNewBet(price, count, 
                                                                  broker, false))); 
        }
        if (buy_offers_.empty()) { 
            buy_offers_.push_front(std::move(NotifyBrokerViaNewBet(price, count, 
                                                                   broker, false))); 
        };
    }
}

void USDExchange::AddSellBet(Broker& broker, double price, double count) {
    for (auto it = buy_offers_.begin(); it != buy_offers_.end();) {
        if (it -> price < price || count == 0) { break; }
        if (it -> count > count) {  
            UpdateBrokerAccount(it -> broker, it -> price, count, false, 
                                it -> bet_token);
            UpdateBrokerAccount(broker, it -> price, count, true, 
                                it -> bet_token);

            it -> count -= count;
            count = 0;
            break;
        } else {
            UpdateBrokerAccount(it -> broker, it -> price, it -> count, false, 
                                it -> bet_token);
            UpdateBrokerAccount(broker, it -> price, it -> count, true, 
                                it -> bet_token);

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
                sell_offers_.insert(it, std::move(NotifyBrokerViaNewBet(price, 
                                                                        count, 
                                                                        broker, 
                                                                        true)));
                return;
            }
        }
        if (it == sell_offers_.end()) { 
            sell_offers_.push_back(std::move(NotifyBrokerViaNewBet(price, 
                                                                   count, 
                                                                   broker, 
                                                                   true))); 
        }
        if (sell_offers_.empty()) { 
            sell_offers_.push_front(std::move(NotifyBrokerViaNewBet(price, 
                                                                    count, 
                                                                    broker, 
                                                                    true)));
        };
    }
}

Bet USDExchange::NotifyBrokerViaNewBet(double price, double count, 
                                       Broker& broker, bool is_sell) {
    std::string bet_token{util::GenerateToken(16)};
    broker.AddCurrentBet(bet_token, count, price, is_sell);
    return {price, count, broker, std::move(bet_token)};
}

void USDExchange::UpdateBrokerAccount(Broker& broker, double price, 
                                      double count, bool is_sell, 
                                      const std::string& token) {
    if (is_sell) {
        broker.AddRUB(price * count);
        broker.AddUSD(-(count));
    } else {
        broker.AddRUB(-(price * count));
        broker.AddUSD(count);  
    }
    broker.AddCompletedBet(count, price, is_sell);
    broker.UpdateCurrentBet(token, count, price);
}

}  // namespace model
