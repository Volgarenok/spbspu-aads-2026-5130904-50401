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

void vasyakin::cmdRemoveItem(
  std::istream& in, std::ostream& out, SystemState& state)
{
  state.completeTransfers();

  const vasyakin::Date date = readDate(in);
  state.current_date_ = date;

  std::string warehouse, model, color;
  size_t size = 0, count = 0;

  warehouse = readQuotedToken(in);
  model = readQuotedToken(in);
  color = readQuotedToken(in);

  if (!(in >> size >> count) || size == 0 || count == 0)
  {
    throw std::runtime_error("Invalid remove-item args");
  }

  if (!state.warehouses_.has(warehouse))
  {
    out << "<ERROR: WAREHOUSE NOT FOUND>" << '\n';
    return;
  }

  std::string key = model + "|" + color + "|" + std::to_string(size);
  auto& wh = state.warehouses_.at(warehouse);
  if (!wh.items_.has(key))
  {
    out << "<ERROR: NOT ENOUGH ITEMS>" << '\n';
    return;
  }

  auto& item = wh.items_.at(key);
  if (!item.removeCount(count))
  {
    out << "<ERROR: NOT ENOUGH ITEMS>" << '\n';
    return;
  }

  wh.used_capacity_ -= count;
  wh.total_value_ -= count * item.getPrice();

  if (item.getCount() == 0)
  {
    wh.items_.remove(key);
  }

  ++state.op_counter_;

  out << "<ITEM REMOVED: " << count << " pcs>" << '\n';

  std::string details = "REMOVE_ITEM " + warehouse + " " + model + " " +
    color + " " + std::to_string(size) + " -" + std::to_string(count);

  state.log_.insert(state.op_counter_,
    LogEntry{state.op_counter_, state.current_date_, details});
}

void vasyakin::cmdShip(
  std::istream& in, std::ostream& out, SystemState& state)
{
  state.completeTransfers();

  std::string from, to, model, color, date;
  size_t count = 0, size = 0;

  from = readQuotedToken(in);
  to = readQuotedToken(in);
  model = readQuotedToken(in);
  color = readQuotedToken(in);

  if (!(in >> size >> count >> date) || size == 0 || count == 0)
  {
    throw std::runtime_error("Invalid ship args");
  }

  vasyakin::Date departure = Date::fromString(date);
  state.current_date_ = departure;

  if (!state.warehouses_.has(from) || !state.warehouses_.has(to))
  {
    out << "<ERROR: WAREHOUSE NOT FOUND>" << '\n';
    return;
  }

  std::string key = model + "|" + color + "|" + std::to_string(size);

  auto& wh_from = state.warehouses_.at(from);
  if (!wh_from.items_.has(key) || wh_from.items_.at(key).getCount() < count)
  {
    out << "<ERROR: NOT ENOUGH ITEMS>" << '\n';
    return;
  }

  size_t travel_days = wh_from.days_to_center_ +
    state.warehouses_.at(to).days_to_center_;

  vasyakin::Date arrival = departure + travel_days;

  auto& wh_to = state.warehouses_.at(to);
  if (wh_to.used_capacity_ + count > wh_to.capacity_)
  {
    out << "<ERROR: NOT ENOUGH CAPACITY AT DESTINATION>" << '\n';
    return;
  }

  auto& item = wh_from.items_.at(key);
  size_t price = item.getPrice();

  item.removeCount(count);
  wh_from.used_capacity_ -= count;
  wh_from.total_value_ -= count * price;

  if (item.getCount() == 0)
  {
    wh_from.items_.remove(key);
  }

  ++state.transfer_counter_;

  state.transfers_.insert(state.transfer_counter_,
    Transfer{state.transfer_counter_, from, to, key,
    count, price, departure, arrival, true});
  
  ++state.op_counter_;

  out << "<TRANSFER #" << state.transfer_counter_ << " CREATED: " << from <<
    " -> " << to << ">" << '\n';

  out << "<Departure: " << departure.toString() <<
    ", Arrival: " << arrival.toString() << " (via Center)>" << '\n';

  std::string details = "SHIP " + from + "->" + to + " " + model + " " + color +
    " " + std::to_string(size) + " -" + std::to_string(count);

  state.log_.insert(state.op_counter_,
    LogEntry{state.op_counter_, state.current_date_, details});
}
