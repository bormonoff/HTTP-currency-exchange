#include "util/util.h"

#include <random>

namespace util {

size_t GetRandomNumber(const int min, const int max) {
    static std::random_device rand{};
    static std::mt19937_64 generator(rand());
    std::uniform_int_distribution<int> distribution{min, max};
    return distribution(generator);
}

std::string GenerateToken(size_t length) {
    std::string result;
    result.reserve(length);
    for (int i = 0; i < length; ++i){
        size_t element = util::GetRandomNumber(1, 15);
        if (element > 9) { 
            if(element == 10){ result += 'a'; continue;}
            if(element == 11){ result += 'b'; continue;}
            if(element == 12){ result += 'c'; continue;}
            if(element == 13){ result += 'd'; continue;}
            if(element == 14){ result += 'e'; continue;}
            if(element == 15){ result += 'f'; continue;}
        }
        result += element + '0';
    }
    return result;
}

}  // namespace util