#include "hash_table.hpp"
#include "hash_func.hpp"
#include "hash_table_iter.hpp"
#include <string>
#include <iostream>
int main()
{
  using namespace malashenko;
  // Hash< std::string > hasher("secret");

  // Equal< std::string > eq;
  HashTable< std::string, int, HmacHash< std::string >, Equal< std::string > > ht(8);
  ht.add("a", 1);
  ht["b"] = 6;
  ht["111"] = 6444;
  ht["2222"] = 99;

  ht["c"] = 10;
  ht["d"] = 2;

  ht.drop("2222");
  ht.drop("111");
  ht.drop("a");
  ht.drop("d");
  ht.drop("b");
  ht.drop("c");

  for (HashTableIter< std::string, int, HmacHash< std::string >, Equal< std::string > > start = ht.begin(); start != ht.end(); ++start)
  {
    std::cout << start->first << ' ' << start->second << '\n';
  }


  // std::cout << ht.get("a") << '\n';
  // std::cout << ht["b"] << '\n';
  // // std::cout << ht.get("aaaa") << '\n';
  // std::cout << ht["c"] << '\n';
  // std::cout << ht["d"] << '\n';

}
