#include <iostream>
#include <string>
#include <cctype>
#include <utility>
#include <stdexcept>
#include <limits>
#include "list.hpp"

namespace petrov
{
  void readData(std::istream & in, petrov::List< std::pair< std::string, petrov::List< size_t > > > & data)
  {
    std::string s;
    while (in >> s)
    {
      petrov::List< size_t > l;
      
      while (in.peek() != EOF && (in.peek() == ' ' || in.peek() == '\n'))
      {
        in.ignore();
      }

      while (in.peek() != EOF && std::isdigit(in.peek()))
      {
        size_t n;
        if (in >> n)
        {
          l.pushBack(n);
        }

        while (in.peek() != EOF && (in.peek() == ' ' || in.peek() == '\n'))
        {
          in.ignore();
        }
      }
      data.pushBack(std::make_pair(s, std::move(l)));
    }
  }

  void printNames(const petrov::List< std::pair< std::string, petrov::List< size_t > > > & data)
  {
    bool f = 1;
    for (petrov::List< std::pair< std::string, petrov::List< size_t > > >::ConstIterator it = data.begin(); it != data.end(); ++it)
    {
      std::cout << (f ? "" : " ") << it->first;
      f = 0;
    }
    std::cout << '\n';
  }

  void process(const petrov::List< std::pair< std::string, petrov::List< size_t > > > & data)
  {
    if (data.isEmpty())
    {
      std::cout << "0\n";
      return;
    }

    petrov::printNames(data);

    petrov::List< petrov::List< size_t >::ConstIterator > its;
    for (petrov::List< std::pair< std::string, petrov::List< size_t > > >::ConstIterator it = data.begin(); it != data.end(); ++it)
    {
      its.pushBack(it->second.begin());
    }

    petrov::List< size_t > res;
    bool go = 1;

    while (go)
    {
      go = 0;
      bool f = 1;
      size_t sum = 0;
      bool ok = 0;

      petrov::List< petrov::List< size_t >::ConstIterator >::Iterator it_n = its.begin();
      for (petrov::List< std::pair< std::string, petrov::List< size_t > > >::ConstIterator it_d = data.begin(); it_d != data.end(); ++it_d, ++it_n)
      {
        if (* it_n != it_d->second.end())
        {
          size_t v = ** it_n;
          std::cout << (f ? "" : " ") << v;
          
          if (std::numeric_limits< size_t >::max() - sum < v)
          {
            throw std::logic_error("over");
          }
          
          sum += v;
          ++(* it_n);
          f = 0;
          go = 1;
          ok = 1;
        }
      }

      if (ok)
      {
        std::cout << "\n";
        res.pushBack(sum);
      }
    }

    bool f_s = 1;
    for (petrov::List< size_t >::ConstIterator it = res.begin(); it != res.end(); ++it)
    {
      std::cout << (f_s ? "" : " ") << * it;
      f_s = 0;
    }
    std::cout << "\n";
  }
}

int main()
{
  try
  {
    petrov::List< std::pair< std::string, petrov::List< size_t > > > d;
    petrov::readData(std::cin, d);
    petrov::process(d);
  }
  catch (const std::overflow_error & e)
  {
    std::cerr << "Owerflow, err\n";
    return 1;
  }
  catch (...)
  {
    std::cerr << "Err\n";
    return 1;
  }
  return 0;
}
