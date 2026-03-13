#include <exception>
#include <iostream>
#include <stdexcept>
#include "io.hpp"

using namespace levkin;
int main()
{
  try {
    Data data;

    readData(std::cin, data);

    if (data.cbegin() != data.cend()) {
      levkin::printNames(std::cout, data);
      levkin::printTransposed(std::cout, data);
    } else {
      std::cout << "0\n";
    }
  } catch (const std::overflow_error &e) {
    std::cerr << e.what() << "\n";
    return 1;
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
  }
}
