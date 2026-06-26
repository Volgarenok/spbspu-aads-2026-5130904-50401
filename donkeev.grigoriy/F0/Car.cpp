#include "Car.hpp"

donkeev::Car::Car():
    vin(""),
    brand(""),
    model(""),
    year(0), 
    color(""),
    bodyType("")
{}

donkeev::Car::Car(const std::string& parametrs)
{
  size_t pos = 0;

  vin = nextWord(parametrs, pos);
  brand = nextWord(parametrs, pos);
  model = nextWord(parametrs, pos);
  
  std::string yearStr = nextWord(parametrs, pos);
  year = std::stoi(yearStr);
  
  color = nextWord(parametrs, pos);
  bodyType = nextWord(parametrs, pos);
}
