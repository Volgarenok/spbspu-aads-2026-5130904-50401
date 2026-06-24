#include "task.hpp"

namespace zhuravleva
{
  Task::Task():
    id(),
    labor(0)
  {}

  Task::Task(const std::string& taskId, size_t taskLabor):
    id(taskId),
    labor(taskLabor)
  {}
}
