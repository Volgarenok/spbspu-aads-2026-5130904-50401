#ifndef FUNC_STACK_QUEUE_HPP
#define FUNC_STACK_QUEUE_HPP

#include "queue.hpp"

namespace zubarev
{
  namespace detail
  {
    Queue< std::string > fromStrToQueue(const std::string& str);
    bool isOperation(const std::string& str);
    size_t getPriority(const std::string& oper);

    Queue< std::string > fromInfixToPostfix(Queue< std::string >& infixQ);
  }

}
#endif
