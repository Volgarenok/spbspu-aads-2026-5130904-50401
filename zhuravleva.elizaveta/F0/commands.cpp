#include "commands.hpp"
#include <fstream>
#include <stdexcept>

namespace zhuravleva
{
  void executeCommand(const std::string& command,
      std::istream& in, std::ostream& out, TaskManager& manager)
  {
    if (command == "create_list")
    {
      std::string listName;
      size_t maxLabor = 0;
      in >> listName >> maxLabor;
      manager.createList(listName, maxLabor);
    }
    else if (command == "delete_list")
    {
      std::string listName;
      in >> listName;
      manager.deleteList(listName);
    }
    else if (command == "add_task_global")
    {
      std::string taskId;
      size_t labor = 0;
      in >> taskId >> labor;
      manager.addTaskGlobal(taskId, labor);
    }
    else if (command == "remove_task_global")
    {
      std::string taskId;
      in >> taskId;
      manager.removeTaskGlobal(taskId);
    }
    else if (command == "add_task_list")
    {
      std::string listName;
      std::string taskId;
      size_t priority = 0;
      in >> listName >> taskId >> priority;
      manager.addTaskToList(listName, taskId, priority);
    }
    else if (command == "remove_task_list")
    {
      std::string listName;
      std::string taskId;
      in >> listName >> taskId;
      manager.removeTaskFromList(listName, taskId);
    }
    else if (command == "show_list")
    {
      std::string listName;
      in >> listName;
      manager.showList(listName, out);
    }
    else if (command == "high_priority")
    {
      std::string listName;
      size_t count = 0;
      in >> listName >> count;
      manager.highPriority(listName, count, out);
    }
    else if (command == "check_add")
    {
      std::string listName;
      std::string taskId;
      in >> listName >> taskId;
      manager.checkAdd(listName, taskId, out);
    }
    else if (command == "suggest_remove")
    {
      std::string listName;
      std::string taskId;
      size_t maxRemove = 0;
      in >> listName >> taskId >> maxRemove;
      manager.suggestRemove(listName, taskId, maxRemove, out);
    }
    else if (command == "optimize")
    {
      std::string newListName;
      std::string listName;
      size_t maxLabor = 0;
      in >> newListName >> listName >> maxLabor;
      manager.optimize(newListName, listName, maxLabor);
    }
    else if (command == "set_deadline")
    {
      std::string listName;
      std::string taskId;
      std::string deadline;
      in >> listName >> taskId >> deadline;
      manager.setDeadline(listName, taskId, deadline);
    }
    else if (command == "show_expired")
    {
      std::string listName;
      std::string date;
      in >> listName >> date;
      manager.showExpired(listName, date, out);
    }
    else if (command == "mark_done")
    {
      std::string listName;
      std::string taskId;
      std::string date;
      in >> listName >> taskId >> date;
      manager.markDone(listName, taskId, date);
    }
    else if (command == "count_completed")
    {
      std::string listName;
      std::string from;
      std::string to;
      in >> listName >> from >> to;
      out << manager.countCompleted(listName, from, to) << "\n";
    }
    else if (command == "capacity")
    {
      std::string listName;
      in >> listName;
      manager.showCapacity(listName, out);
    }
    else if (command == "save")
    {
      std::string fileName;
      in >> fileName;
      std::ofstream file(fileName.c_str());
      if (!file.is_open())
      {
        throw std::logic_error("file error");
      }
      manager.save(file);
    }
    else if (command == "load")
    {
      std::string fileName;
      in >> fileName;
      std::ifstream file(fileName.c_str());
      if (!file.is_open())
      {
        throw std::logic_error("file error");
      }
      manager.load(file);
    }
    else
    {
      throw std::logic_error("invalid command");
    }
  }
}
