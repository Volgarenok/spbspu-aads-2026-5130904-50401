#include "commands.hpp"

void donkeev::printCar(const Car& car)
{
  std::cout << car.brand << " " << car.model
  << " (" << car.year << ") | VIN: " << car.vin
  << " | " << car.color
  << " | " << car.bodyType<< "\n";
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

size_t donkeev::getHistorySize()
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

void donkeev::handleScroll(CarTable&, AdTable& ads, const std::string&)
{
  std::cout << "\n\033[1;33m=== ЛЕНТА ОБЪЯВЛЕНИЙ ===\033[0m\n";
  
  if (ads.empty())
  {
    std::cout << "  \033[90mНет активных объявлений.\033[0m\n\n";
    return;
  }
  
  int count = 0;
  for (auto it = ads.begin(); it != ads.end(); ++it)
  {
    if (it->value_.isActive())
    {
      ++count;
      std::cout << "\n  \033[36m━━━ " << count << ". ━━━\033[0m\n";
      
      const Ad& ad = it->value_;
      const Car& car = ad.getCar();
      std::cout << "  ID: " << ad.getId() << "\n";
      std::cout << "  VIN: " << car.vin << "\n";
      std::cout << "  Марка: " << car.brand << "\n";
      std::cout << "  Модель: " << car.model << "\n";
      std::cout << "  Год: " << car.model << "\n";
      std::cout << "  Цвет: " << car.color << "\n";
      std::cout << "  Цена: " << ad.getPrice() << " руб.\n";
      std::cout << "  Пробег: " << ad.getMileage() << " км\n";
      std::cout << "  Владельцев: " << ad.getOwnerCount() << "\n";
      std::cout << "  Статус: \033[32mАктивно\033[0m\n";
    }
  }
  
  std::cout << "\n  \033[90mВсего активных объявлений: " << count << "\033[0m\n\n";
}

void donkeev::handleMake(CarTable& cars, AdTable& ads, const std::string&)
{
  const std::string CARS_FILE = "cars.txt";
  
  std::cout << "\n\033[1;33m=== СОЗДАНИЕ ОБЪЯВЛЕНИЯ ===\033[0m\n";
  
  std::string vin = inputString("  VIN машины: ");
  
  Car* car = cars.find(vin);
  
  if (!car)
  {
    std::cout << "\033[33m  Машина с таким VIN не найдена.\033[0m\n";
    std::cout << "  Введите данные новой машины:\n";
    
    std::string brand = inputString("    Марка: ");
    std::string model = inputString("    Модель: ");
    int year = inputInt("    Год выпуска: ");
    std::string color = inputString("    Цвет: ");
    std::string bodyType = inputString("    Тип кузова: ");
    
    Car newCar;
    newCar.vin = vin;
    newCar.brand = brand;
    newCar.model = model;
    newCar.year = year;
    newCar.color = color;
    newCar.bodyType = bodyType;
    cars.insert(vin, newCar);
    
    saveCarToDataBase(newCar, CARS_FILE);
    
    std::cout << "\033[32m  ✓ Машина добавлена в базу!\033[0m\n";
    
    car = cars.find(vin);
  }
  
  std::cout << "\n  \033[1;33mВведите данные объявления:\033[0m\n";
  
  size_t price = inputSize_t("    Цена (руб.): ");
  size_t mileage = inputSize_t("    Пробег (км): ");
  size_t ownerCount = inputSize_t("    Количество владельцев: ");
  
  size_t adId = donkeev::generateNextAdId(ads, getHistorySize());
  Ad ad;
  ad.setId(adId);
  ad.setCar(car);
  ad.setPrice(price);
  ad.setMileage(mileage);
  ad.setOwnerCount(ownerCount);
  ad.setActive(true);
  
  ads.insert(ad.getIdNumber(), ad);
  
  std::cout << "\033[32m  ✓ Объявление создано! ID: " << ad.getId() << "\033[0m\n\n";
}
void donkeev::handleBuy(CarTable&, AdTable& ads, const std::string&)
{
  const std::string HISTORY_FILE = "history.txt";
  
  std::cout << "\033[31m  Укажите ID\033[0m\n\n";

  std::string idStr;
  std::getline(std::cin, idStr);
  
  if (idStr.empty())
  {
    std::cout << "\033[31m  Ошибка\033[0m\n\n";
    return;
  }
  
  while (!idStr.empty() && idStr[0] == '0')
  {
    idStr.erase(0, 1);
  }
  
  size_t id = std::stoull(idStr);
  
  Ad* ad = ads.find(id);
  
  if (!ad)
  {
    std::cout << "\033[31m  Объявление не найдено.\033[0m\n\n";
    return;
  }
  
  if (!ad->isActive())
  {
    std::cout << "\033[31m  Это объявление уже продано.\033[0m\n\n";
    return;
  }
  
  saveAdToHistory(*ad);
  ads.remove(id);
  
  std::cout << "\033[32m  ✓ Поздравляем с покупкой!\033[0m\n";
}
void donkeev::handleDelete(CarTable&, AdTable& ads, const std::string&)
{
  std::cout << "\n  \033[33mВведите ID объявления для удаления: \033[0m";
  
  std::string idStr;
  std::getline(std::cin, idStr);
  
  if (idStr.empty())
  {
    std::cout << "\033[31m  Ошибка: ID не может быть пустым.\033[0m\n\n";
    return;
  }
  
  while (!idStr.empty() && idStr[0] == '0')
  {
    idStr.erase(0, 1);
  }
  
  size_t id = std::stoull(idStr);
  
  Ad* ad = ads.find(id);
  
  if (!ad)
  {
    std::cout << "\033[31m  Объявление не найдено.\033[0m\n\n";
    return;
  }
  
  if (!ad->isActive())
  {
    std::cout << "\033[31m  Это объявление уже продано. Удаление невозможно.\033[0m\n\n";
    return;
  }
  
  ads.remove(id);
  std::cout << "\033[32m  ✓ Объявление " << idStr << " удалено.\033[0m\n\n";
}

