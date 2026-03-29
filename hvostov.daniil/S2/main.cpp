#include <iostream>
#include "arifmetic.hpp"
#include <fstream>

int main(int argc, char ** argv)
{
  hvostov::Stack< long long int > result;
  try {
    if (argc > 1) {
      std::ifstream in(argv[1]);
      result = hvostov::getResult(in);
    } else {
      result = hvostov::getResult(std::cin);
    }
  } catch (const std::logic_error& e) {
    std::cerr << e.what() << "\n";
    return 1;
  } catch(const std::overflow_error& e) {
    std::cerr << e.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "Unknown error!\n";
    return 1;
  }
  hvostov::printResult(result);
  return 0;
}
