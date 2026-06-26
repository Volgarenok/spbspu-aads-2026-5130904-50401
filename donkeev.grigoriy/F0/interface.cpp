#include <iostream>

#include "interface.hpp"

void donkeev::showBanner()
{
  std::cout << "\n";
  std::cout << "  \033[36m═══════════════════════════════════════════════════════════════\033[0m\n";
  std::cout << "     \033[1;33m🚗  АГРЕГАТОР ОБЪЯВЛЕНИЙ АВТОМОБИЛЕЙ  🚗\033[0m\n";
  std::cout << "  \033[36m═══════════════════════════════════════════════════════════════\033[0m\n";
  std::cout << "\n";
}

void donkeev::showNavigation()
{
  std::cout << "  \033[1;33mОСНОВНЫЕ ДЕЙСТВИЯ:\033[0m\n";
  std::cout << "\n";
  std::cout << "    \033[32mscroll\033[0m        — Показать ленту объявлений\n";
  std::cout << "    \033[32mmake\033[0m          — Создать объявление\n";
  std::cout << "    \033[32mbuy\033[0m           — Купить автомобиль по ID\n";
  std::cout << "    \033[32mdelete\033[0m        — Удалить объявление по ID\n";
  std::cout << "    \033[32mshow-istory\033[0m   — Показать историю объявлений\n";
  std::cout << "    \033[32mquit\033[0m          — Выйти из программы\n";
  std::cout << "\n";
}