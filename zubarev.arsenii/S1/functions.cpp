#include "all.hpp"
#include <string>
#include <iostream>

namespace zubarev
{
  List< Data > input(std::istream& in)
  {
    List< Data > list;
    LIter< Data > itList = list.before_begin();
    Data value;

    while (in >> value.name) {
      List< int > nums;
      LIter< int > itNum = nums.before_begin();
      int num;


      while (true) {

        while (in.peek() == ' ' || in.peek() == '\t') {
          in.get();
        }


        if (in.peek() == '\n' || in.eof()) {
          if (in.peek() == '\n') {
            in.get();
          }
          break;
        }


        if (in >> num) {
          nums.insert_after(itNum, num);
        } else {
          break; 
        }
      }

      value.numbers = nums;
      list.insert_after(itList, value);
    }

    return list;
  }
}
