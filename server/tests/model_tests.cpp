#include "model/exchange.h"

#include <catch2/catch_test_macros.hpp>

SCENARIO("exchange") {
    GIVEN("the exchange class with brokers") {
        model::USDExchange ex;
        auto& broker1 = ex.AddBroker();
        auto& broker2 = ex.AddBroker();
        auto& broker3 = ex.AddBroker();
        auto& broker4 = ex.AddBroker();
        auto& broker5 = ex.AddBroker();
        auto& broker6 = ex.AddBroker();
        
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

        WHEN("borkers add bets") {
            THEN("brokers firtsly sell old bets") {
                ex.AddSellBet(broker2, 50, 50);
                ex.AddSellBet(broker1, 50, 50);
                ex.AddBuyBet(broker3, 50, 50);

                CHECK(broker2.GetUSD() == -50);
                CHECK(broker1.GetUSD() == 0);
            }
        }

        WHEN("borkers add bets") {
            THEN("brokers firtsly buy old bets") {
                ex.AddBuyBet(broker2, 50, 50);
                ex.AddBuyBet(broker1, 50, 50);
                ex.AddSellBet(broker3, 50, 50);

                CHECK(broker2.GetUSD() == 50);
                CHECK(broker1.GetUSD() == 0);
            }
        }

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

        WHEN("borkers add sell orders") {
            THEN("brokers orders update correctly") {
                ex.AddSellBet(broker1, 48, 20);
                ex.AddSellBet(broker2, 49, 20);
                CHECK((broker1.GetBrokerBets().find((*ex.GetSellOffers().begin()).bet_token) != broker1.GetBrokerBets().end()));
                CHECK((broker2.GetBrokerBets().find((*std::next(ex.GetSellOffers().begin())).bet_token) != broker2.GetBrokerBets().end()));
                
                AND_WHEN("brokers buy USD") {
                    THEN("bets remove sucessfully") {
                        ex.AddBuyBet(broker3, 49, 20);
                        ex.AddBuyBet(broker3, 49, 10);
                        ex.AddBuyBet(broker3, 49, 5);
                        CHECK(broker1.GetBrokerBets().empty());
                        CHECK(broker2.GetBrokerBets().size() == 1);
                        CHECK(broker2.GetCompleteBrokerBets().size() == 2);
                        CHECK(broker3.GetBrokerBets().size() == 0);
                        CHECK(broker3.GetCompleteBrokerBets().size() == 3);
                        
                        AND_THEN("Broker after several bets has valid bets") {
                            CHECK((*broker2.GetBrokerBets().begin()).second.count == 5);
                        }
                    }
                }
            }
        }

        WHEN("borkers add buy orders") {
            THEN("brokers orders update correctly") {
                ex.AddBuyBet(broker1, 49, 20);
                ex.AddBuyBet(broker2, 48, 20);
                CHECK((broker1.GetBrokerBets().find((*ex.GetBuyOffers().begin()).bet_token) != broker1.GetBrokerBets().end()));
                CHECK((broker2.GetBrokerBets().find((*std::next(ex.GetBuyOffers().begin())).bet_token) != broker2.GetBrokerBets().end()));
                
                AND_WHEN("brokers sell USD") {
                    THEN("bets remove sucessfully") {
                        ex.AddSellBet(broker3, 48, 20);
                        ex.AddSellBet(broker3, 48, 10);
                        ex.AddSellBet(broker3, 48, 5);
                        CHECK(broker1.GetBrokerBets().empty());
                        CHECK(broker2.GetBrokerBets().size() == 1);
                        CHECK(broker2.GetCompleteBrokerBets().size() == 2);
                        CHECK(broker3.GetBrokerBets().size() == 0);
                        CHECK(broker3.GetCompleteBrokerBets().size() == 3);
                        
                        AND_THEN("Broker after several bets has valid bets") {
                            CHECK((*broker2.GetBrokerBets().begin()).second.count == 5);
                        }
                    }
                }
            }
        }
    }
}
