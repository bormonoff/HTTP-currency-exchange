#include "model/exchange.h"

#include <catch2/catch_test_macros.hpp>

SCENARIO("exchange") {
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
                std::list<double> expect {49, 50, 51.5, 61.2};
                auto it_expect = expect.begin();
                for (auto it : ex.GetSellOffers()) {
                    CHECK(it.price == (*it_expect++));
                }
            }
        }
    }

    GIVEN("the exchange class with brokers") {
        model::USDExchange ex;
        auto& broker1 = ex.AddBroker();
        auto& broker2 = ex.AddBroker();
        auto& broker3 = ex.AddBroker();
        auto& broker4 = ex.AddBroker();


        WHEN("borkers add buy bets") {
            THEN("bets add correctly") {
                ex.AddBuyBet(broker1, 50, 50);
                ex.AddBuyBet(broker2, 49, 50);
                ex.AddBuyBet(broker3, 51.5, 50);
                ex.AddBuyBet(broker4, 28.5, 50);
                ex.AddBuyBet(broker4, 61.2, 50);
                std::list<double> expect {61.2, 51.5, 50, 49};
                auto it_expect = expect.begin();
                for (auto it : ex.GetBuyOffers()) {
                    CHECK(it.price == (*it_expect++));
                }
            }
        }
    }

    GIVEN("the exchange class with brokers") {
        model::USDExchange ex;
        auto& broker1 = ex.AddBroker();
        auto& broker2 = ex.AddBroker();
        auto& broker3 = ex.AddBroker();

        WHEN("borkers add bets") {
            THEN("brokers firtsly sell old bets") {
                ex.AddSellBet(broker2, 50, 50);
                ex.AddSellBet(broker1, 50, 50);
                ex.AddBuyBet(broker3, 50, 50);

                CHECK(broker2.GetUSD() == -50);
                CHECK(broker1.GetUSD() == 0);
            }
        }
    }

    GIVEN("the exchange class with brokers") {
        model::USDExchange ex;
        auto& broker1 = ex.AddBroker();
        auto& broker2 = ex.AddBroker();
        auto& broker3 = ex.AddBroker();

        WHEN("borkers add bets") {
            THEN("brokers firtsly buy old bets") {
                ex.AddBuyBet(broker2, 50, 50);
                ex.AddBuyBet(broker1, 50, 50);
                ex.AddSellBet(broker3, 50, 50);

                CHECK(broker2.GetUSD() == 50);
                CHECK(broker1.GetUSD() == 0);
            }
        }
    }

    GIVEN("the exchange class with brokers") {
        model::USDExchange ex;
        auto& broker1 = ex.AddBroker();
        auto& broker2 = ex.AddBroker();
        auto& broker3 = ex.AddBroker();
        auto& broker4 = ex.AddBroker();
        auto& broker5 = ex.AddBroker();
        auto& broker6 = ex.AddBroker();

        WHEN("borkers add different bets") {
            THEN("bets match correctly") {
                ex.AddSellBet(broker1, 48, 20);
                ex.AddBuyBet(broker2, 35, 20);
                ex.AddSellBet(broker3, 50, 30);
                ex.AddBuyBet(broker4, 49, 25);
                ex.AddSellBet(broker5, 30, 20);
                ex.AddBuyBet(broker6, 70, 20);
                CHECK(((broker1.GetUSD() == -20) && (broker1.GetRoubles() == 980)));
                CHECK(((broker2.GetUSD() ==  15) && (broker2.GetRoubles() == -525)));
                CHECK(((broker3.GetUSD() == -20) && (broker3.GetRoubles() == 1400)));
                CHECK(((broker4.GetUSD() ==  25) && (broker4.GetRoubles() == -1225)));
                CHECK(((broker5.GetUSD() == -20) && (broker5.GetRoubles() == 770)));
                CHECK(((broker6.GetUSD() ==  20) && (broker6.GetRoubles() == -1400)));

                AND_THEN("exchange class has correct balance of sell bets") {
                    CHECK((*ex.GetSellOffers().begin()).price == 50);
                    CHECK((*ex.GetSellOffers().begin()).count == 10);
                }

                AND_THEN("exchange class has correct balance of buy bets") {
                    CHECK((*ex.GetBuyOffers().begin()).price == 35);
                    CHECK((*ex.GetBuyOffers().begin()).count == 5);
                }
            }
        }
    }

    GIVEN("the exchange class with brokers") {
        model::USDExchange ex;
        auto& broker1 = ex.AddBroker();
        auto& broker2 = ex.AddBroker();
        auto& broker3 = ex.AddBroker();

        WHEN("borkers add different bets") {
            THEN("brokers bets update correctly") {
                ex.AddSellBet(broker1, 48, 20);
                ex.AddSellBet(broker2, 49, 20);
                CHECK((broker1.GetBrokerBets().find((*ex.GetSellOffers().begin()).bet_token) != broker1.GetBrokerBets().end()));
                CHECK((broker2.GetBrokerBets().find((*std::next(ex.GetSellOffers().begin())).bet_token) != broker2.GetBrokerBets().end()));
            }
            AND_WHEN("brokers buy their bets") {
                THEN("bets remove sucessfully") {
                    ex.AddSellBet(broker3, 49, 20);
                    ex.AddSellBet(broker3, 49, 10);


                }
            }
        }
    }
}
