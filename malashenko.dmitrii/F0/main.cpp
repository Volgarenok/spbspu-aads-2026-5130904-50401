#include <iostream>
#include "cuckoo_hash_table.hpp"
#include "hash_funcs.hpp"
#include "equal_functor.hpp"

int main()
{

  using namespace malashenko;
  CuckooHashTable< int, char, HmacHash< int >, SipHasher< int >, Equal< char > > t;
  t.add(1, 'a');
  t.add(2, 'b');
  t.add(3, 'c');
  t.add(4, 'd');
  t.add(5, 'e');
  t.add(6, 'f');
  t.add(7, 'g');

  t.add(8, 'h');
  t.add(9, 'i');
  t.add(10, 'j');
  t.add(11, 'k');
  t.add(12, 'l');

  t.add(13, 'a');
  t.add(14, 'b');
  t.add(15, 'c');
  t.add(16, 'd');
  t.add(17, 'e');
  t.add(18, 'f');
  t.add(19, 'g');

  t.add(20, 'h');


  std::cout << t[1] << '\n';
  std::cout << t[2] << '\n';
  std::cout << t[3] << '\n';
  std::cout << t[4] << '\n';
  std::cout << t[5] << '\n';
  std::cout << t[6] << '\n';
  std::cout << t[7] << '\n';
  std::cout << t[8] << '\n';
  std::cout << t[9] << '\n';
  std::cout << t[10] << '\n';
  std::cout << t[11] << '\n';
  std::cout << t[12] << '\n';
  std::cout << t[13] << '\n';
  std::cout << t[14] << '\n';
  std::cout << t[15] << '\n';
  std::cout << t[16] << '\n';
  std::cout << t[17] << '\n';
  std::cout << t[18] << '\n';
  std::cout << t[19] << '\n';
  std::cout << t[20] << '\n';

  t.clear();
  std::cout << t.size() << '\n';


}
