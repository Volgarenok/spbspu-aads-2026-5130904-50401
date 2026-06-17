#include "utils.hpp"

vasyakin::WarehouseState::WarehouseState() noexcept:
  capacity_(0),
  days_to_center_(0),
  used_capacity_(0),
  items_sent_(0),
  items_received_(0),
  total_value_(0)
{}

vasyakin::WarehouseState::WarehouseState(
  const std::string& name, size_t capacity, size_t days_to_center):
  name_(name),
  capacity_(capacity),
  days_to_center_(days_to_center),
  used_capacity_(0),
  items_sent_(0),
  items_received_(0),
  total_value_(0)
{}

vasyakin::SystemState::SystemState() noexcept:
  op_counter_(0),
  transfer_counter_(0),
  current_date_(2026, 1, 1)
{}

void vasyakin::SystemState::completeTransfers()
{
  for (auto it = transfers_.begin(); it != transfers_.end(); ++it)
  {
    auto key = (*it).first;
    auto& tr = transfers_.at(key);

    if (tr.active_ && tr.arrival_ <= current_date_)
    {
      tr.active_ = false;

      auto& wh_to = warehouses_.at(tr.to_);
      auto& wh_from = warehouses_.at(tr.from_);

      wh_to.items_received_ += tr.count_;
      wh_from.items_sent_ += tr.count_;

      if (wh_to.items_.has(tr.item_key_))
      {
        wh_to.items_.at(tr.item_key_).addCount(tr.count_);
      }
      else
      {
        std::string model = tr.item_key_.substr(0, tr.item_key_.find('|'));
        std::string rest = tr.item_key_.substr(tr.item_key_.find('|') + 1);
        std::string color = rest.substr(0, rest.find('|'));
        size_t size = std::stoul(rest.substr(rest.find('|') + 1));

        wh_to.items_.insert(tr.item_key_,
          Item(model, color, size, tr.count_, tr.price_));
      }

      wh_to.used_capacity_ += tr.count_;
      wh_to.total_value_ += tr.count_ * tr.price_;
    }
  }
}
