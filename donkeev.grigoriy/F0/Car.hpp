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

  Car::Car():
    vin(""),
    brand(""),
    model(""),
    year(0), 
    color(""),
    bodyType("")
  {}

  Car::Car(const std::string& parametrs)
  {
    size_t pos = 0;

    vin = nextWord(parametrs, pos);
    brand = nextWord(parametrs, pos);
    model = nextWord(parametrs, pos);
    
    std::string yearStr = nextWord(parametrs, pos);
    year = std::stoi(yearStr);
    
    color = nextWord(parametrs, pos);
    bodyType = nextWord(parametrs, pos);
    
    std::string volumeStr = nextWord(parametrs, pos);
  }
}

#endif
