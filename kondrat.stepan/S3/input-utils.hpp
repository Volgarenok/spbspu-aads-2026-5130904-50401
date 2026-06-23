#ifndef INPUT_UTILS_HPP
#define INPUT_UTILS_HPP

#include <istream>

namespace kondrat
{
  namespace detail
  {
    template< class T >
    bool readArgument(std::istream & in, T & value)
    {
      using Traits = std::char_traits< char >;

      Traits::int_type current = in.peek();
      while (current == ' ' || current == '\t' || current == '\r')
      {
        in.get();
        current = in.peek();
      }
      if (current == '\n' || Traits::eq_int_type(current, Traits::eof()))
      {
        return false;
      }
      return static_cast< bool >(in >> value);
    }
  }
}

#endif
