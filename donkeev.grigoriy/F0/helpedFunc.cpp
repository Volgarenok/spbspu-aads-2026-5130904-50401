#include "helpedFunc.hpp"

std::string donkeev::nextWord(const std::string& str, size_t& pos)
{
  while (pos < str.length() && str[pos] == ' ')
  {
    ++pos;
  }

  if (pos >= str.length())
  {
    return "";
  }
  
  size_t start = pos;
  while (pos < str.length() && str[pos] != ' ')
  {
    ++pos;
  }
  
  return str.substr(start, pos - start);
}

int inputInt(const std::string& prompt)
{
    int value;
    std::cout << prompt;
    std::cin >> value;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

std::string donkeev::inputString(const std::string& prompt)
{
  std::string value;
  std::cout << prompt;
  std::getline(std::cin, value);
  return value;
}

void donkeev::saveCarToDataBase(const Car& car, const std::string& filename)
{
  std::ofstream file(filename, std::ios::app);
  if (!file.is_open())
  {
    throw std::runtime_error("Bad file");
    return;
  }
  
  std::string line = car.vin + " " + car.brand + " " + car.model;
  
  file << line << "\n";
  file.close();
}
