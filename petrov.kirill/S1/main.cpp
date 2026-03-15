#include <iostream>
#include <string>
#include "list.hpp"

namespace petrov {
  void testBasicList()
  {
    List< std::string > test_list;

    test_list.pushBack("first");
    test_list.pushBack("second");
    test_list.pushBack("third");

    std::cout << "List size: " << test_list.getSize() << "\n";
    std::cout << "Last element: " << test_list.getBack() << "\n";

    std::cout << "Contents: ";
    for (auto it = test_list.begin(); it != test_list.end(); ++it) {
      std::cout << * it << " ";
    }
    std::cout << "\n";

    test_list.clear();
    if (test_list.isEmpty()) {
      std::cout << "List is now empty\n";
    }
  }
}

int main()
{
  try {
    petrov::testBasicList();
  } catch (const std::exception & e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
