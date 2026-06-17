#include "commands.hpp"
#include <fstream>
#include <limits>
#include <cctype>

vasyakin::Date readDate(std::istream& in)
{
  std::string token;
  if (!(in >> token))
  {
    throw std::runtime_error("Invalid date");
  }

  return vasyakin::Date::fromString(token);
}

std::string readQuotedToken(std::istream& in)
{
  std::string token;
  in >> token;

  if (token.empty())
  {
    return token;
  }

  if (token.front() == '"')
  {
    token = token.substr(1);

    if (!token.empty() && token.back() == '"')
    {
      return token.substr(0, token.size() - 1);
    }

    std::string rest;
    std::getline(in, rest, '"');
    token += rest;
  }

  return token;
}

std::string escapeSpaces(const std::string& s)
{
  std::string result = s;
  for (char& c : result)
  {
    if (c == ' ') c = '_';
  }

  return result;
}

std::string unescapeSpaces(const std::string& s)
{
  std::string result = s;
  for (char& c : result)
  {
    if (c == '_') c = ' ';
  }

  return result;
}

void vasyakin::cmdCreateWarehouse(
  std::istream& in, std::ostream& out, SystemState& state)
{
  state.completeTransfers();

  const vasyakin::Date date = readDate(in);
  state.current_date_ = date;

  std::string name;
  size_t capacity = 0, days_to_center = 0;

  if (!(in >> name >> capacity >> days_to_center) ||
    capacity == 0 || days_to_center == 0)
  {
    throw std::runtime_error("Invalid create-warehouse args");
  }

  if (state.warehouses_.has(name))
  {
    out << "<ERROR: WAREHOUSE ALREADY EXISTS>" << '\n';
    return;
  }

  state.warehouses_.insert(name,
    WarehouseState(name, capacity, days_to_center));

  ++state.op_counter_;

  out << "<WAREHOUSE CREATED: " << name
    << " (capacity: " << capacity
    << ", days-to-center: " << days_to_center << ")>" << '\n';

  state.log_.insert(state.op_counter_,
    LogEntry{state.op_counter_, state.current_date_, "CREATE_WAREHOUSE " + name});
}

void vasyakin::cmdAddItem(
  std::istream& in, std::ostream& out, SystemState& state)
{
  state.completeTransfers();

  const vasyakin::Date date = readDate(in);
  state.current_date_ = date;

  std::string warehouse, model, color;
  size_t size = 0, count = 0, price = 0;

  warehouse = readQuotedToken(in);
  model = readQuotedToken(in);
  color = readQuotedToken(in);

  if (!(in >> size >> count >> price) ||
    size == 0 || count == 0 || price == 0)
  {
    throw std::runtime_error("Invalid add-item args");
  }

  if (!state.warehouses_.has(warehouse))
  {
    out << "<ERROR: WAREHOUSE NOT FOUND>" << '\n';
    return;
  }

  auto& wh = state.warehouses_.at(warehouse);
  if (wh.used_capacity_ + count > wh.capacity_)
  {
    out << "<ERROR: WAREHOUSE CAPACITY EXCEEDED>" << '\n';
    return;
  }

  std::string key = model + "|" + color + "|" + std::to_string(size);
  if (wh.items_.has(key))
  {
    wh.items_.at(key).addCount(count);
  }
  else
  {
    wh.items_.insert(key, Item(model, color, size, count, price));
  }

  wh.used_capacity_ += count;
  wh.total_value_ += count * price;
  ++state.op_counter_;

  out << "<ITEM ADDED: " << count << " pcs>" << '\n';

  std::string details = "ADD_ITEM " + warehouse + " " + model + " " +
    color + " " + std::to_string(size) + " +" + std::to_string(count);

  state.log_.insert(state.op_counter_,
    LogEntry{state.op_counter_, state.current_date_, details});
}
