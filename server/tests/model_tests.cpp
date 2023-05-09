#include "model/exchange.h"

#include <catch2/catch_test_macros.hpp>

SCENARIO("model") {
    GIVEN("the exchange class with brokers") {
        model::USDExchange ex;
        auto& broker1 = ex.AddBroker();
        auto& broker2 = ex.AddBroker();
        auto& broker3 = ex.AddBroker();
        auto& broker4 = ex.AddBroker();

        WHEN("borkers add sell bets") {
            THEN("bets add correctly") {
                ex.AddSellBet(broker1, 50, 50);
                ex.AddSellBet(broker2, 49, 50);
                ex.AddSellBet(broker3, 51.5, 50);
                ex.AddSellBet(broker4, 28.5, 50);
                ex.AddSellBet(broker4, 61.2, 50);
            }
        }

        WHEN("borkers add buy bets") {
            THEN("bets add correctly") {
                ex.AddBuyBet(broker1, 50, 50);
                ex.AddBuyBet(broker2, 49, 50);
                ex.AddBuyBet(broker3, 51.5, 50);
                ex.AddBuyBet(broker4, 28.5, 50);
                ex.AddBuyBet(broker4, 61.2, 50);
            }
        }
    }
}