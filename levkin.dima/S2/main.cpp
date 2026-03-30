#include <exception>
#include <iostream>

int main()
{
  try {
    std::cout << "init";
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
