#include "Car.hpp"

donkeev::Car::Car():
    vin_(""),
    brand_(""),
    model_(""),
    year_(0),
    color_(""),
    bodyType_("")
{}

donkeev::Car::Car(const std::string& parametrs)
{
  size_t pos = 0;

  vin_ = nextWord(parametrs, pos);
  brand_ = nextWord(parametrs, pos);
  model_ = nextWord(parametrs, pos);

  std::string yearStr = nextWord(parametrs, pos);
  year_ = std::stoi(yearStr);

  color_ = nextWord(parametrs, pos);
  bodyType_ = nextWord(parametrs, pos);
}
