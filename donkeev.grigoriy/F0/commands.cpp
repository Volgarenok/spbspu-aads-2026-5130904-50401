#include "commands.hpp"

void donkeev::printCar(const Car& car)
{
  std::cout << car.brand_ << " " << car.model_
  << " (" << car.year_ << ") | VIN: " << car.vin_
  << " | " << car.color_
  << " | " << car.bodyType_<< "\n";
}

void donkeev::saveCarToDataBase(const Car& car, const std::string& filename)
{
  std::ofstream file(filename, std::ios::app);
  if (!file.is_open())
  {
    throw std::runtime_error("Bad file");
    return;
  }

  std::string line = car.vin_ + " " + car.brand_ + " " + car.model_;

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
  result += "VIN: " + ad.getCar().vin_ + "\n";
  result += "Марка: " + ad.getCar().brand_ + "\n";
  result += "Модель: " + ad.getCar().model_ + "\n";
  result += "Год: " + std::to_string(ad.getCar().year_) + "\n";
  result += "Цвет: " + ad.getCar().color_ + "\n";
  result += "Тип кузова: " + ad.getCar().bodyType_ + "\n";
  result += "Цена: " + std::to_string(ad.getPrice()) + "\n";
  result += "Пробег: " + std::to_string(ad.getMileage()) + "\n";
  result += "Владельцев: " + std::to_string(ad.getOwnerCount()) + "\n";
  result += "Bought\n";

  file << result << "\n";
  file.close();
}

void donkeev::printAd(const Ad& ad, size_t& count)
{
  ++count;
  std::cout << "\n  \033[36m━━━ " << count << ". ━━━\033[0m\n";

  const Car& car = ad.getCar();
  std::cout << "  ID: " << ad.getId() << "\n";
  std::cout << "  VIN: " << car.vin_ << "\n";
  std::cout << "  Марка: " << car.brand_ << "\n";
  std::cout << "  Модель: " << car.model_ << "\n";
  std::cout << "  Год: " << car.model_ << "\n";
  std::cout << "  Цвет: " << car.color_ << "\n";
  std::cout << "  Цена: " << ad.getPrice() << " руб.\n";
  std::cout << "  Пробег: " << ad.getMileage() << " км\n";
  std::cout << "  Владельцев: " << ad.getOwnerCount() << "\n";
  std::cout << "  Статус: \033[32mАктивно\033[0m\n";
}

void donkeev::handleScroll(CarTable&, AdTable& ads)
{
  std::cout << "\n\033[1;33m=== ЛЕНТА ОБЪЯВЛЕНИЙ ===\033[0m\n";

  if (ads.empty())
  {
    std::cout << "  \033[90mНет активных объявлений.\033[0m\n\n";
    return;
  }

  size_t count = 0;
  for (AdIterator it = ads.begin(); it != ads.end(); ++it)
  {
    if ((*it).isActive())
    {
      printAd((*it), count);
    }
  }

  std::cout << "\n  \033[90mВсего активных объявлений: " << count << "\033[0m\n\n";
}

void donkeev::handleSort(CarTable&, AdTable& ads)
{
  std::cout << "\n  \033[33mВыберите параметр сортировки:\033[0m\n";
  std::cout << "    \033[32mbrand\033[0m       — по марке\n";
  std::cout << "    \033[32mbrand-model\033[0m — по марке и модели\n";
  std::cout << "    \033[32mprice\033[0m       — по цене\n";

  std::string type;
  std::getline(std::cin, type);

  if (type == "brand")
  {
    std::cout << "\n  \033[33mВведите марку\033[0m\n";
    std::string brand;
    std::getline(std::cin, brand);

    AdIterator begin = ads.begin();
    AdIterator end = ads.end();
    size_t count = 0;
    while (begin != end)
    {
      if ((*begin).getCar().brand_ == brand)
      {
        printAd((*begin), count);
      }
      ++begin;
    }

    if (count == 0)
    {
      std::cout << "\n\033[33mНет подходящих объвлений\033[0m\n\n";
    }

    return;
  }
  else if (type == "brand-model")
  {
    std::cout << "\n  \033[33mВведите марку и модель через пробел\033[0m\n";
    std::string line;
    std::getline(std::cin, line);

    std::string brand;
    std::string model;
    size_t pos = 0;
    brand = nextWord(line, pos);
    model = nextWord(line, pos);

    AdIterator begin = ads.begin();
    AdIterator end = ads.end();
    size_t count = 0;
    while (begin != end)
    {
      const Car& car = (*begin).getCar();
      if (car.brand_ == brand && car.model_ == model)
      {
        printAd((*begin), count);
      }
      ++begin;
    }

    if (count == 0)
    {
      std::cout << "\n\033[33mНет подходящих объвлений\033[0m\n\n";
    }

    return;
  }
  else if (type == "price")
  {
    Ad** activeAds = new Ad*[ads.size()];
    size_t index = 0;
    for (AdIterator it = ads.begin(); it != ads.end(); ++it)
    {
      activeAds[index++] = &(*it);
    }

    for (size_t i = 1; i < ads.size(); ++i)
    {
      Ad* key = activeAds[i];
      size_t j = i;

      while (j > 0 && activeAds[j - 1]->getPrice() > key->getPrice())
      {
        activeAds[j] = activeAds[j - 1];
        --j;
      }
      activeAds[j] = key;
    }

    for (size_t i = 0; i < ads.size();)
    {
      printAd(*(activeAds[i]), i);
    }

    delete [] activeAds;
    return;
  }
  else
  {
    std::cout << "\033[31m  Неверная команда\033[0m\n\n";
    return;
  }

}

void donkeev::handleMake(CarTable& cars, AdTable& ads)
{
  const std::string CARS_FILE = "cars.txt";

  std::cout << "\n\033[1;33m=== СОЗДАНИЕ ОБЪЯВЛЕНИЯ ===\033[0m\n";

  std::string vin = inputString("  VIN машины: ");

  if (!donkeev::isDigit(vin))
  {
    std::cout << "Неверный vin\n";
    return;
  }

  Car* car = cars.find(vin);

  if (!car)
  {
    std::cout << "\033[33m  Машина с таким VIN не найдена.\033[0m\n";
    std::cout << "  Введите данные новой машины:\n";

    std::string brand = inputString("    Марка: ");
    std::string model = inputString("    Модель: ");
    std::string yearStr = inputString("    Год выпуска: ");
    std::string color = inputString("    Цвет: ");
    std::string bodyType = inputString("    Тип кузова: ");

    if (!donkeev::isDigit(yearStr) || !donkeev::isAlpha(color) ||
      !donkeev::isAlpha(bodyType))
    {
      std::cout << "Неверные параметры\n";
      return;
    }

    size_t year = std::stoull(yearStr);
    Car newCar;
    newCar.vin_ = vin;
    newCar.brand_ = brand;
    newCar.model_ = model;
    newCar.year_ = year;
    newCar.color_ = color;
    newCar.bodyType_ = bodyType;
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
void donkeev::handleBuy(CarTable&, AdTable& ads)
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
void donkeev::handleDelete(CarTable&, AdTable& ads)
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
void donkeev::handleShowHistory(CarTable&, AdTable&)
{
  const std::string HISTORY_FILE = "history.txt";

  std::cout << "\n\033[1;33m=== ИСТОРИЯ ПРОДАННЫХ АВТО ===\033[0m\n";

  std::ifstream file(HISTORY_FILE);
  if (!file.is_open())
  {
    std::cout << "  \033[90mИстория пуста.\033[0m\n\n";
    return;
  }

  std::string line;
  bool found = false;
  int count = 0;

  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }

    if (line.find("ID:") != std::string::npos)
    {
      ++count;
      std::cout << "\n  \033[36m━━━ " << count << ". ━━━\033[0m\n";
    }

    std::cout << "  " << line << "\n";
    found = true;
  }

  if (!found)
  {
    std::cout << "  \033[90mИстория пуста.\033[0m\n";
  }

  file.close();
  std::cout << "\n";
}

void donkeev::handleChoozeBest(CarTable&, AdTable& ads)
{
  using PriceTable = donkeev::RobinTable< std::string, size_t, StringHash, StringEqual >;

  PriceTable prices(16);
  prices.insert("bmw m5", 8500);
  prices.insert("audi rs6", 6700);
  prices.insert("mersedes s-class", 7000);
  prices.insert("toyota camry", 4000);
  prices.insert("mersedes e-class", 3700);
  prices.insert("kia sorento", 3400);
  prices.insert("nissan x-trail", 2700);
  prices.insert("mini cooper", 1600);
  prices.insert("kia rio", 1000);
  prices.insert("vaz 2105", 300);

  std::cout << "\n  \033[33mВведите бюджет\033[0m\n";
  std::string budgetStr;
  std::getline(std::cin, budgetStr);
  if (!isDigit(budgetStr))
  {
    std::cout << "\033[31m  Неверный бюджет\033[0m\n\n";
    return;
  }

  size_t budget = std::stoull(budgetStr);

  Ad** activeAds = new Ad*[ads.size()];
  size_t index = 0;
  for (AdIterator it = ads.begin(); it != ads.end(); ++it)
  {
    if (it->isActive())
    {
      activeAds[index++] = &(*it);
    }
  }

  size_t* profits = new size_t[ads.size()];
  double* efficiency = new double[ads.size()];

  for (size_t i = 0; i < ads.size(); ++i)
  {
    const Car& car = activeAds[i]->getCar();
    std::string priceKey = std::string(car.brand_) + " " + std::string(car.model_);
    size_t marketPrice = prices.at(priceKey);
    size_t adPrice = activeAds[i]->getPrice();

    profits[i] = (marketPrice > adPrice) ? (marketPrice - adPrice) : 0;
    efficiency[i] = (adPrice > 0) ? static_cast<double>(profits[i]) / adPrice : 0;
  }

  for (size_t i = 1; i < ads.size(); ++i)
  {
    Ad* keyAd = activeAds[i];
    size_t keyProfit = profits[i];
    double keyEff = efficiency[i];

    size_t j = i;
    while (j > 0 && efficiency[j - 1] < keyEff)
    {
      activeAds[j] = activeAds[j - 1];
      profits[j] = profits[j - 1];
      efficiency[j] = efficiency[j - 1];
      --j;
    }
    activeAds[j] = keyAd;
    profits[j] = keyProfit;
    efficiency[j] = keyEff;
  }

  size_t totalSpent = 0;
  size_t totalProfit = 0;
  size_t selectedCount = 0;

  std::cout << "\n  \033[1;33mВыбранные автомобили:\033[0m\n";
  for (size_t i = 0; i < ads.size(); ++i)
  {
    size_t price = activeAds[i]->getPrice();

    if (totalSpent + price <= budget)
    {
      totalSpent += price;
      totalProfit += profits[i];

      printAd(*(activeAds[i]), selectedCount);
    }
  }

  std::cout << "\n  \033[1;33m=== РЕЗУЛЬТАТ ПОДБОРА ===\033[0m\n";
  std::cout << "  Бюджет: " << budget << " тыс. руб.\n";
  std::cout << "  Потрачено: " << totalSpent << " тыс. руб.\n";
  std::cout << "  Куплено: " << selectedCount << " авто\n";
  std::cout << "  Общая прибыль: " << totalProfit << " тыс. руб.\n";

  delete[] activeAds;
  delete[] profits;
  delete[] efficiency;
}
