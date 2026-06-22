#ifndef MENU_HPP
#define MENU_HPP
#include <iostream>
#include <string>
#include <rh_hash_table.hpp>

namespace zharov
{
  struct CafeSystem;

  struct MenuItem
  {
    int price;
    int prep_time;
    std::string description;
    bool available;
  };

  using MenuTable =
    RHHashTable< std::string, MenuItem, std::hash< std::string >, std::equal_to< std::string > >;
  using Menus =
    RHHashTable< std::string, MenuTable, std::hash< std::string >, std::equal_to< std::string > >;

  void menuAdd(std::ostream&, std::istream&, CafeSystem&);
  void menuRemove(std::ostream&, std::istream&, CafeSystem&);
  void menuPrice(std::ostream&, std::istream&, CafeSystem&);
  void menuTime(std::ostream&, std::istream&, CafeSystem&);
  void menuDesc(std::ostream&, std::istream&, CafeSystem&);
  void menuAvailable(std::ostream&, std::istream&, CafeSystem&);
  void menuImport(std::ostream&, std::istream&, CafeSystem&);
  void menuCopy(std::ostream&, std::istream&, CafeSystem&);

  void menuShow(std::ostream&, std::istream&, const CafeSystem&);
  void menuExport(std::ostream&, std::istream&, const CafeSystem&);
  void menuList(std::ostream&, std::istream&, const CafeSystem&);
}
#endif
