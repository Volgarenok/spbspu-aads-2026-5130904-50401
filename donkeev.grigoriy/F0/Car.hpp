#ifndef CAR_HPP
#define CAR_HPP

#include <string>
#include <iostream>

#include "../common/top-it-vector.hpp"
#include "helpedFunc.hpp"

namespace donkeev
{
struct Car
{
    std::string vin_;
    std::string brand_;
    std::string model_;
    int year_;
    std::string color_;
    std::string bodyType_;

    Car();

    explicit Car(const std::string&);
};
}

#endif
