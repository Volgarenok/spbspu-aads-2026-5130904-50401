#include "FOO.hpp"
#include <istream>
#include <iostream>

namespace petrov
{
  void mkData(std::istream& input, List< std::pair< std::string, List< size_t > > >& data)
  {
    std::string name;
    while(input >> name)
    {
      List< size_t > nums;
      size_t num;
      LIter< size_t > inPos = nums.getLast();
      while (input >> num)
      {
        inPos = nums.insert(inPos, num);
      }
      data.insert(data.getLast(), {name, nums});

      if(input.eof())
      {
        break;
      }
      else
      {
        input.clear();
      }
    }
  }

  size_t maxLen(const List< std::pair< std::string, List< size_t > > >& data)
  {
    size_t max = 0;
    LCIter< std::pair< std::string, List< size_t > > > it = data.begin();
    while (it.hasNext())
    {
      max = std::max(max, (*it).second.size());
      it = it.next();
    }
    return max;
  }

  bool canShowItem(const List< size_t >& nums, size_t istr)
  {
    return nums.size() >= istr;
  }

  void printAllList(List< std::pair< std::string, List< size_t > > >& data)
  {
    LIter< std::pair< std::string, List< size_t > > > outIt = data.begin();
    while (outIt.hasNext())
    {
      std::cout << (*outIt).first << " ";
      outIt = outIt.next();
    }
    std::cout << "\n";

    LIter< size_t > last = (*data.end()).second.end();
    LIter< size_t > now = (*data.begin()).second.begin();
    
    while (now != last)
    {
      outIt = data.begin();

    }
    
  }
}
