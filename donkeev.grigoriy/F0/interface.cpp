#include <iostream>

#include "interface.hpp"

void donkeev::showBanner()
{
  std::cout << "\n\033[1;33m=== CarAggregator ===\033[0m\n\n\n";
}

void donkeev::showNavigation()
{
  std::cout << "\033[1;33mОСНОВНЫЕ ДЕЙСТВИЯ:\033[0m\n";
  std::cout << "\n";
  std::cout << "\033[32mscroll\033[0m";
  std::cout << "        — Показать ленту объявлений\n";
  std::cout << "\033[32mmake\033[0m";
  std::cout << "          — Создать объявление\n";
  std::cout << "\033[32mbuy\033[0m";
  std::cout << "           — Купить автомобиль по ID\n";
  std::cout << "\033[32mdelete\033[0m";
  std::cout << "        — Удалить объявление по ID\n";
  std::cout << "\033[32mshow-history\033[0m";
  std::cout << "   — Показать историю объявлений\n";
  std::cout << "\033[32mquit\033[0m";
  std::cout << "          — Выйти из программы\n";
  std::cout << "\n";
}