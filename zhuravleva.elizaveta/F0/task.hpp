#ifndef TASK_HPP
#define TASK_HPP
#include <cstddef>
#include <string>

namespace zhuravleva
{
  struct Task
  {
    std::string id;
    size_t labor;

    Task();
    Task(const std::string& taskId, size_t taskLabor);
  };
}

#endif
