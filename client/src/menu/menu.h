#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <sstream>

namespace menu {

using namespace std::literals;

class Menu {
public:
    using Handler = std::function<bool()>;

    explicit Menu(std::istream& input, std::ostream& output) 
        : input_{input}
        , output_{output} {
    }

    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;
    
    void AddAction(std::string action_name, std::string description, Handler handler);

    void Run();

    void ShowInsructions();

private:
    struct Action {
        Handler handler;
        std::string description;
    };

    bool ParseCommand(std::istream& input);

    std::istream& input_;
    std::ostream& output_;
    std::map<std::string, Action> actions_;
};

}  // namespace menu