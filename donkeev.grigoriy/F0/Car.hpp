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
    std::string vin;
    std::string brand;
    std::string model;
    int year;
    std::string color;
    std::string bodyType;
    
    Car();

    explicit Car(const std::string&);
};
}

#endif
