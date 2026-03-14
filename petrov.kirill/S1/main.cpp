#include <iostream>
#include <string>
#include "list.hpp"

int main() {
  petrov::List<std::string> raw_data;
  std::string token;
  while (std::cin >> token) {
    raw_data.push_back(token);
  }

  for (auto* n = raw_data.head; n; n = n->next) {
    std::cout << n->data << " ";
  }
  return 0;
}