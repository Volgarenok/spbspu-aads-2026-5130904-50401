#include "commands.hpp"

void donkeev::printCar(const Car& car)
{
    std::cout << car.brand << " " << car.model
    << " (" << car.year << ") | VIN: " << car.vin
    << " | " << car.color
    << " | " << car.bodyType<< "\n";
}


void donkeev::handleScroll(CarTable&, AdTable& ads, const std::string&)
{
  std::cout << "\n\033[1;33m=== ЛЕНТА ОБЪЯВЛЕНИЙ ===\033[0m\n";
  
  bool found = false;
  int count = 0;
  
  for (AdIterator it = ads.begin(); it != ads.end(); ++it)
  {
    if (it->value_.isActive())
    {
      ++count;
      std::cout << "  \033[36m" << count << ".\033[0m ";
      printCar(it->value_.getCar());
      found = true;
    }
  }
  
  if (!found)
  {
    std::cout << "  \033[90mНет активных объявлений.\033[0m\n";
  }
  else
  {
    std::cout << "\n  \033[90mВсего активных объявлений: " << count << "\033[0m\n";
  }

  std::cout << "\n";
}
void donkeev::handleMake(CarTable& , AdTable& , const std::string& )
{

}
void donkeev::handleBuy(CarTable& , AdTable&, const std::string&)
{

}
void donkeev::handleDelete(CarTable& , AdTable& , const std::string&)
{

}
void donkeev::handleShowHistory(CarTable& , AdTable& , const std::string&)
{

}