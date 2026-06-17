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
