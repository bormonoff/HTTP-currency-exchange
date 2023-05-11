#include "menu/menu.h"

#include <iomanip>

namespace menu {
    
bool Menu::ParseCommand(std::istream& input) {
    using namespace std::literals;

    try {
        std::string cmd;
        if (input >> cmd) {
            if (const auto it = actions_.find(cmd); it != actions_.cend()) {
                if (!it->second.handler()) {
                    return false;
                }
            } else {
                output_ << "Command '"sv << cmd << "' has not been found."sv << std::endl;
            }
        } else {
            if (cmd == "") { return true; }
            output_ << "Invalid command"sv << std::endl;   
        }
    } catch (const std::exception& e) {
        output_ << e.what() << std::endl;
    }
    return true;
}

void Menu::AddAction(std::string action_name, std::string description, Handler handler) {
    if (!actions_.try_emplace(std::move(action_name), std::move(handler), std::move(description)).second) {
        std::cout << "A command has been added already"s << std::endl;
    }
}

void Menu::Run() {
    std::string line;
    while (std::getline(input_, line)) {
        std::istringstream cmd_stream{std::move(line)};
        if (!ParseCommand(cmd_stream)) {
            break;
        }
    }
}

void Menu::ShowInsructions() {
    if (actions_.empty()) {
        return;
    }
    size_t width{};
    for (const auto& [action, info] : actions_) {
        width = std::max(width, action.length());
    }
    
    output_ << '\n' << std::left << std::setfill(' ');
    for (const auto& [action, info] : actions_) {
        output_ <<  std::setw(width + 1) << action
                << ' ' << info.description << std::endl;
    }
}
}  // namespace menu
