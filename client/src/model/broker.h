#pragma once

#include <string>

namespace model {

class Broker {
public:
    Broker() = default;

    Broker(const Broker&) = delete;
    Broker& operator=(const Broker&) = delete;

    void SetToken(std::string&& token) { token_ = std::move(token); }
    
    const std::string& GetToken() const noexcept { return token_; }

private:
    std::string token_;
};
}  // namespace model