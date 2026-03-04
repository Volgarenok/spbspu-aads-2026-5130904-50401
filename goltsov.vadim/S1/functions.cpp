#include "functions.hpp"
#include <iostream>

namespace goltsov
{
  List< std::pair< std::string, List< size_t > > > getData(std::istream& in, size_t& size)
  {
    List< std::pair< std::string, List< size_t > > > result;
    LIter< std::pair< std::string, List< size_t > > > i(nullptr);
    std::string name;
    while (in >> name)
    {
      size += 1;
      List< size_t > numbers;
      size_t number;
      LIter< size_t > j(nullptr);
      while (in >> number)
      {
        j = numbers.insert(j, number);
        char next = in.peek();
        if (next == '\n' || next == EOF)
        {
          break;
        }
      }
      if (!in.eof())
      {
        in.clear();
      }
      i = result.insert(i, {name, numbers});
    }
    return result;
  }

  void push_back(size_t** sums, size_t& n, size_t a)
  {
    size_t* new_sums = new size_t[n + 1];
    for (size_t i = 0; i < n; ++i)
    {
      new_sums[i] = sums[0][i];
    }
    new_sums[n] = a;
    delete[] (* sums);
    sums[0] = new_sums;
    n += 1;
  }

  std::ostream& printResult(std::ostream& out, List< std::pair< std::string, List< size_t > > >& data, size_t size)
  {
    LIter< std::pair< std::string, List< size_t > > > it = data.begin();
    size_t* sums = nullptr;
    size_t n = 0;
    LIter< size_t >* its = new LIter< size_t >[size];
    for (size_t i = 0; i < size; ++i)
    {
      if (i != size - 1)
      {
        out << (* it).first << ' ';
      }
      else
      {
        out << (* it).first << '\n';
      }
      try
      {
        its[i] = (* it).second.begin();
      }
      catch(...)
      {
        delete[] its;
        delete[] sums;
      }
      it = it.next();
    }
    it = data.begin();
    bool all = 0;
    size_t real_ind = 0;
    while (!all)
    {
      all = 1;
      size_t sum = 0;
      for (size_t i = 0; i < size; ++i)
      {
        if (its[i].hasNext())
        {
          if (all == 1)
          {
            out << (* its[i]);
            sum += (* its[i]);
          }
          else
          {
            out << ' ' << (* its[i]);
            sum += (* its[i]);
          }
          all = 0;
          its[i] = its[i].next();
        }
      }
      if (!all)
      {
        try
        {
          push_back(& sums, n, sum);
        }
        catch(...)
        {
          delete[] sums;
          delete[] its;
        }
        out << '\n';
      }
    }
    for (size_t i = 0; i < n; ++i)
    {
      if (i != n - 1)
      {
        out << sums[i] << ' ';
      }
      else
      {
        out << sums[i] << '\n';
      }
    }
  }
}
