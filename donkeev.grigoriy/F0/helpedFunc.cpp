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

int inputSize_t(const std::string& prompt)
{
  size_t value;
  std::cout << prompt;
  std::cin >> value;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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

size_t getHistorySize()
{
  const std::string HISTORY_FILE = "history.txt";
  
  std::ifstream file(HISTORY_FILE);
  if (!file.is_open())
  {
    return 0;
  }
  
  size_t count = 0;
  std::string line;
    
  while (std::getline(file, line))
  {
    if (line.find("ID:") != std::string::npos)
    {
      ++count;
    }
  }
  
  file.close();
  return count;
}

size_t donkeev::generateNextAdId(AdTable& ads, size_t histroySize)
{
  size_t candidate = ads.size() + histroySize + 1;
  
  while (ads.contains(candidate))
  {
    ++candidate;
  }
  
  return candidate;
}

void donkeev::saveAdToHistory(const Ad& ad)
{
  const std::string filename = "history.txt";
  std::ofstream file(filename, std::ios::app);
  if (!file.is_open())
  {
    throw std::runtime_error("Bad file");
    return;
  }
  
  std::string result;
    
  result += "ID: " + ad.getId() + "\n";
  result += "VIN: " + ad.getCar().vin + "\n";
  result += "Марка: " + ad.getCar().brand + "\n";
  result += "Модель: " + ad.getCar().model + "\n";
  result += "Год: " + std::to_string(ad.getCar().year) + "\n";
  result += "Цвет: " + ad.getCar().color + "\n";
  result += "Тип кузова: " + ad.getCar().bodyType + "\n";
  result += "Цена: " + std::to_string(ad.getPrice()) + "\n";
  result += "Пробег: " + std::to_string(ad.getMileage()) + "\n";
  result += "Владельцев: " + std::to_string(ad.getOwnerCount()) + "\n";
  result += "Bought\n";

  file << result << "\n";
  file.close();
}
