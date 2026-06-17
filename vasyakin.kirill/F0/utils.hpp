#ifndef UTILS_HPP
#define UTILS_HPP
#include "item.hpp"
#include "date.hpp"
#include "AVLTree.hpp"

namespace vasyakin
{
  struct WarehouseState
  {
    WarehouseState() noexcept;
    WarehouseState(const std::string& name, size_t capacity, size_t days_to_center);

    std::string name_;
    size_t capacity_;
    size_t days_to_center_;
    AVLTree< std::string, Item > items_;
    size_t used_capacity_;
    size_t items_sent_;
    size_t items_received_;
    size_t total_value_;
  };

  struct Transfer
  {
    size_t id_;
    std::string from_;
    std::string to_;
    std::string item_key_;
    size_t count_;
    size_t price_;
    Date departure_;
    Date arrival_;
    bool active_;
  };

  struct LogEntry
  {
    size_t id_;
    Date date_;
    std::string details_;
  };

  struct SystemState
  {
    SystemState() noexcept;

    void completeTransfers();

    AVLTree< std::string, WarehouseState > warehouses_;
    AVLTree< size_t, Transfer > transfers_;
    AVLTree< size_t, LogEntry > log_;
    size_t op_counter_;
    size_t transfer_counter_;
    Date current_date_;
  };
}

#endif
