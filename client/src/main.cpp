#include "menu/menu.h"
#include "scenarios/scenario.h"
#include "handler/handler.h"

namespace {

void Start() {
    using namespace std::literals;
    menu::Menu menu{std::cin, std::cout};
    menu.AddAction("Help"s, "Show instructions"s, [&menu]() {
        menu.ShowInsructions();
        return true;
    });
    menu.AddAction("Exit"s, "Exit program"s, [&menu]() {
        return false;
    });
    network::Network network;
    scenario::Scenario scenario{menu, network, std::cin, std::cout};
    menu.Run();
}
}  // namespace 

int main() {
    Start();
}
