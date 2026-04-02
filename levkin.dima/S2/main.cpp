#include <exception>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
  try {

    std::ifstream file;
    if (argc > 1) {
      file.open(argv[1]);
      if (!file) {
        std::cerr << "bad file";
        return 1;
      }
    }
    // std::istream& in = (argc > 1) ? file : std::cin;

   
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
