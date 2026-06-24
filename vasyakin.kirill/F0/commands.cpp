#include "commands.hpp"
#include <fstream>
#include <limits>
#include <cctype>

namespace
{
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
      if (c == ' ')
      {
        c = '_';
      }
    }

    return result;
  }

  std::string unescapeSpaces(const std::string& s)
  {
    std::string result = s;
    for (char& c : result)
    {
      if (c == '_')
      {
        c = ' ';
      }
    }

    return result;
  }
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

  if (state.warehouses_.count(name))
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

  if (!state.warehouses_.count(warehouse))
  {
    out << "<ERROR: WAREHOUSE NOT FOUND>" << '\n';
    return;
  }

  vasyakin::WarehouseState& wh = state.warehouses_.at(warehouse);
  if (wh.used_capacity_ + count > wh.capacity_)
  {
    out << "<ERROR: WAREHOUSE CAPACITY EXCEEDED>" << '\n';
    return;
  }

  std::string key = model + "|" + color + "|" + std::to_string(size);
  if (wh.items_.count(key))
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

  if (!state.warehouses_.count(warehouse))
  {
    out << "<ERROR: WAREHOUSE NOT FOUND>" << '\n';
    return;
  }

  std::string key = model + "|" + color + "|" + std::to_string(size);
  vasyakin::WarehouseState& wh = state.warehouses_.at(warehouse);
  if (!wh.items_.count(key))
  {
    out << "<ERROR: NOT ENOUGH ITEMS>" << '\n';
    return;
  }

  vasyakin::Item& item = wh.items_.at(key);
  if (!item.removeCount(count))
  {
    out << "<ERROR: NOT ENOUGH ITEMS>" << '\n';
    return;
  }

  wh.used_capacity_ -= count;
  wh.total_value_ -= count * item.getPrice();

  if (item.getCount() == 0)
  {
    wh.items_.erase(key);
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

  if (!state.warehouses_.count(from) || !state.warehouses_.count(to))
  {
    out << "<ERROR: WAREHOUSE NOT FOUND>" << '\n';
    return;
  }

  std::string key = model + "|" + color + "|" + std::to_string(size);

  vasyakin::WarehouseState& wh_from = state.warehouses_.at(from);
  if (!wh_from.items_.count(key) || wh_from.items_.at(key).getCount() < count)
  {
    out << "<ERROR: NOT ENOUGH ITEMS>" << '\n';
    return;
  }

  size_t travel_days = wh_from.days_to_center_ +
    state.warehouses_.at(to).days_to_center_;

  vasyakin::Date arrival = departure + travel_days;

  vasyakin::WarehouseState& wh_to = state.warehouses_.at(to);
  if (wh_to.used_capacity_ + count > wh_to.capacity_)
  {
    out << "<ERROR: NOT ENOUGH CAPACITY AT DESTINATION>" << '\n';
    return;
  }

  vasyakin::Item& item = wh_from.items_.at(key);
  size_t price = item.getPrice();

  item.removeCount(count);
  wh_from.used_capacity_ -= count;
  wh_from.total_value_ -= count * price;

  if (item.getCount() == 0)
  {
    wh_from.items_.erase(key);
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

void vasyakin::cmdShowWarehouse(
  std::istream& in, std::ostream& out, SystemState& state)
{
  state.completeTransfers();

  std::string name;
  if (!(in >> name))
  {
    throw std::runtime_error("Invalid show-warehouse args");
  }

  const vasyakin::Date date = readDate(in);
  state.current_date_ = date;

  if (!state.warehouses_.count(name))
  {
    out << "<ERROR: WAREHOUSE NOT FOUND>" << '\n';
    return;
  }

  out << "<WAREHOUSE: " << name << " (Date: " << date.toString() << ")>" << '\n';

  const vasyakin::WarehouseState& wh = state.warehouses_.at(name);
  size_t total_items = 0;

  for (auto cit = wh.items_.cbegin(); cit != wh.items_.cend(); ++cit)
  {
    out << "> " << cit->second.toString() << '\n';
    total_items += cit->second.getCount();
  }

  out << "TOTAL: " << total_items << " pcs | USED CAPACITY: "
    << wh.used_capacity_ << "/" << wh.capacity_ << '\n';
}

void vasyakin::cmdShowTransfers(
  std::istream& in, std::ostream& out, SystemState& state)
{
  state.completeTransfers();

  const vasyakin::Date date = readDate(in);
  state.current_date_ = date;

  out << "<ACTIVE TRANSFERS (Date: " <<
    state.current_date_.toString() << ")>" <<'\n';

  size_t active_trans = 0;

  for (auto cit = state.transfers_.cbegin(); cit != state.transfers_.cend(); ++cit)
  {
    const vasyakin::Transfer& tr = cit->second;

    if (tr.active_ && tr.departure_ <= date && tr.arrival_ > date)
    {
      out << "> #" << tr.id_ << ": " << tr.from_ << " -> " << tr.to_
        << " | " << tr.item_key_.substr(0, tr.item_key_.find('|'))
        << " | " << tr.count_ << " pcs | Status: In Transit" << '\n';

      ++active_trans;
    }
  }

  out << "TOTAL ACTIVE: " << active_trans << '\n';
}

void vasyakin::cmdShowItem(
  std::istream& in, std::ostream& out, SystemState& state)
{
  std::string model, color;
  size_t size = 0;

  model = readQuotedToken(in);
  color = readQuotedToken(in);

  if (!(in >> size) || size == 0)
  {
    throw std::runtime_error("Invalid show-item args");
  }

  std::string search_log = model + " " + color + " " + std::to_string(size);
  std::string key_items = model + "|" + color + "|" + std::to_string(size);

  out << "<ITEM HISTORY: " << model << " " << color << " " << size << ">" << '\n';

  for (auto cit = state.log_.cbegin(); cit != state.log_.cend(); ++cit)
  {
    const vasyakin::LogEntry& log = cit->second;

    if (log.details_.find(search_log) != std::string::npos)
    {
      out << "> [" << log.date_.toString() << "] " << log.details_
        << " [Op #" << log.id_ << "]" << '\n';
    }
  }

  out << "CURRENT LOCATIONS (" << state.current_date_.toString() << "): ";

  bool found = false;
  for (auto cit = state.warehouses_.cbegin(); cit != state.warehouses_.cend(); ++cit)
  {
    const std::string& wh1 = cit->first;
    const vasyakin::WarehouseState& wh2 = cit->second;

    if (wh2.items_.count(key_items))
    {
      if (found)
      {
        out << ", ";
      }

      out << wh1 << " (" << wh2.items_.at(key_items).getCount() << " pcs)";
      found = true;
    }
  }

  if (!found)
  {
    out << "None";
  }

  out << '\n';
}

void vasyakin::cmdShowLog(
  std::istream& in, std::ostream& out, SystemState& state)
{
  int limit = 0;

  int ch = in.peek();
  while (ch == ' ' || ch == '\t')
  {
    in.get();
    ch = in.peek();
  }

  if (ch != EOF && std::isdigit(static_cast< unsigned char >(ch)))
  {
    in >> limit;
  }

  if (limit > 0)
  {
    out << "<OPERATION LOG (LAST " << limit << ")>" << '\n';
  }
  else
  {
    out << "<OPERATION LOG>" << '\n';
  }

  size_t count = 0;
  size_t total = state.log_.size();

  size_t start = (limit > 0 &&
    total > static_cast< size_t >(limit)) ? total - limit : 0;

  for (auto cit = state.log_.cbegin(); cit != state.log_.cend(); ++cit)
  {
    if (count >= start)
    {
      const vasyakin::LogEntry& log = cit->second;

      out << "> #" << log.id_ << " [" << log.date_.toString()
        << "] " << log.details_ << '\n';
    }

    ++count;
  }
}

void vasyakin::cmdShowStateAt(
  std::istream& in, std::ostream& out, SystemState& state)
{
  state.completeTransfers();

  const vasyakin::Date date = readDate(in);
  state.current_date_ = date;

  size_t total = 0;
  size_t in_transit = 0;
  size_t active = 0;

  for (auto cit = state.warehouses_.cbegin(); cit != state.warehouses_.cend(); ++cit)
  {
    const vasyakin::WarehouseState& wh = cit->second;

    for (auto ccit = wh.items_.cbegin(); ccit != wh.items_.cend(); ++ccit)
    {
      total += ccit->second.getCount();
    }
  }

  for (auto cit = state.transfers_.cbegin(); cit != state.transfers_.cend(); ++cit)
  {
    const vasyakin::Transfer& tr = cit->second;

    if (tr.departure_ <= date && tr.arrival_ > date)
    {
      in_transit += tr.count_;
      ++active;
    }
  }

  out << "<SYSTEM STATE (Date: " << date.toString() << ")>" << '\n';
  out << "> Warehouses: " << state.warehouses_.size() << '\n';
  out << "> Total Items: " << total << '\n';
  out << "> Items in Transit: " << in_transit << '\n';
  out << "> Active Transfers: " << active << '\n';
}

void vasyakin::cmdStats(
  std::istream& in, std::ostream& out, SystemState& state)
{
  state.completeTransfers();

  std::string warehouse;

  int ch = in.peek();
  while (ch == ' ' || ch == '\t')
  {
    in.get();
    ch = in.peek();
  }

  if (ch != EOF && ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r')
  {
    in >> warehouse;
  }

  size_t total_items = 0;
  if (warehouse.empty())
  {
    out << "<SYSTEM STATS>" << '\n';
    out << "> Warehouses: " << state.warehouses_.size() << '\n';

    size_t completed = 0;
    size_t active = 0;

    for (auto cit = state.warehouses_.cbegin(); cit != state.warehouses_.cend(); ++cit)
    {
      const vasyakin::WarehouseState& wh = cit->second;

      for (auto ccit = wh.items_.cbegin(); ccit != wh.items_.cend(); ++ccit)
      {
        total_items += ccit->second.getCount();
      }
    }

    for (auto cit = state.transfers_.cbegin(); cit != state.transfers_.cend(); ++cit)
    {
      if (cit->second.arrival_ <= state.current_date_)
      {
        ++completed;
      }
      else
      {
        ++active;
      }
    }

    out << "> Total Items (all warehouses): " << total_items << '\n';
    out << "> Total Transfers Completed: " << completed << '\n';
    out << "> Total Transfers Active: " << active << '\n';
  }
  else
  {
    if (!state.warehouses_.count(warehouse))
    {
      out << "<ERROR: WAREHOUSE NOT FOUND>" << '\n';
      return;
    }

    const vasyakin::WarehouseState& wh = state.warehouses_.at(warehouse);
    for (auto cit = wh.items_.cbegin(); cit != wh.items_.cend(); ++cit)
    {
      total_items += cit->second.getCount();
    }

    out << "<WAREHOUSE STATS: " << warehouse << ">" << '\n';
    out << "> Total Items: " << total_items << '\n';
    out << "> Total Value: " << wh.total_value_ << '\n';
    out << "> Items Sent: " << wh.items_sent_ << '\n';
    out << "> Items Received: " << wh.items_received_ << '\n';
    out << "> Capacity Used: " << wh.used_capacity_ << "/"
      << wh.capacity_ << " (" << (wh.capacity_ > 0 ?
      (wh.used_capacity_ * 100 / wh.capacity_) : 0) << "%)" << '\n';
  }
}

void vasyakin::cmdCalculateCenterCapacity(
  std::istream& in, std::ostream& out, SystemState& state)
{
  state.completeTransfers();

  const vasyakin::Date from = readDate(in);
  const vasyakin::Date to = readDate(in);
  state.current_date_ = to;

  if (from > to)
  {
    throw std::runtime_error("Invalid date range: from > to");
  }

  size_t peak_load = 0;
  vasyakin::Date peak_date = from;

  for (vasyakin::Date curr = from; curr <= to; curr = curr + 1)
  {
    size_t daily_load = 0;

    for (auto cit = state.transfers_.cbegin(); cit != state.transfers_.cend(); ++cit)
    {
      const vasyakin::Transfer& tr = cit->second;

      if (!state.warehouses_.count(tr.from_) || !state.warehouses_.count(tr.to_))
      {
        continue;
      }

      size_t days_from = state.warehouses_.at(tr.from_).days_to_center_;
      vasyakin::Date at_center_date = tr.departure_ + days_from;

      if (curr == at_center_date)
      {
        daily_load += tr.count_;
      }
    }

    if (daily_load > peak_load)
    {
      peak_load = daily_load;
      peak_date = curr;
    }
  }

  out << "<CENTER CAPACITY ANALYSIS (" << from.toString()
    << " — " << to.toString() << ")>" << '\n';
  out << "> Peak Date: " << peak_date.toString() << '\n';
  out << "> Peak Load: " << peak_load << " pcs (at center simultaneously)" << '\n';
  out << "> Total Shipments: " << state.transfers_.size() << '\n';
  out << "> MINIMUM REQUIRED CAPACITY: " << peak_load << " pcs" << '\n';
}

void vasyakin::cmdHelp(
  std::istream&, std::ostream& out, SystemState&)
{
  out << "<AVAILABLE COMMANDS>" << '\n';
  out << "> create-warehouse <date> <name> <capacity> <days-to-center>" << '\n';
  out << "> add-item <date> <warehouse> <model> <color> <size> <count> <price>" << '\n';
  out << "> remove-item <date> <warehouse> <model> <color> <size> <count>" << '\n';
  out << "> ship <from> <to> <model> <color> <size> <count> <departure-date>" << '\n';
  out << "> show-warehouse <name> <date>" << '\n';
  out << "> show-transfers <date>" << '\n';
  out << "> show-item <model> <color> <size>" << '\n';
  out << "> show-log [limit]" << '\n';
  out << "> stats [warehouse]" << '\n';
  out << "> calculate-center-capacity <date-from> <date-to>" << '\n';
  out << "> show-state-at <date>" << '\n';
  out << "> help" << '\n';
  out << "> save <filename>" << '\n';
  out << "> load <filename>" << '\n';
}

void vasyakin::cmdSave(
  std::istream& in, std::ostream& out, SystemState& state)
{
  std::string filename;
  if (!(in >> filename))
  {
    throw std::runtime_error("Invalid save args");
  }

  std::ofstream file(filename);
  if (!file.is_open())
  {
    out << "<ERROR: Cannot save file>" << '\n';
    return;
  }

  file << "# Привет, держи отчёт о проделанной работе!" << '\n';

  file << "META " << state.op_counter_ << ' ' << state.transfer_counter_ << ' '
    << state.current_date_.toString() << '\n';

  for (auto cit = state.warehouses_.cbegin(); cit != state.warehouses_.cend(); ++cit)
  {
    const std::string& wh_name = cit->first;
    const vasyakin::WarehouseState& wh = cit->second;

    file << "WH " << wh_name << ' ' << wh.capacity_ << ' '
      << wh.days_to_center_ << ' ' << wh.used_capacity_ << ' '
      << wh.items_sent_ << ' ' << wh.items_received_ << ' '
      << wh.total_value_ << '\n';

    for (auto ccit = wh.items_.cbegin(); ccit != wh.items_.cend(); ++ccit)
    {
      const vasyakin::Item& item = ccit->second;

      file << "IT " << wh_name << ' '
        << escapeSpaces(item.getModel()) << ' '
        << escapeSpaces(item.getColor()) << ' '
        << item.getSize() << ' '
        << item.getCount() << ' '
        << item.getPrice() << '\n';
    }
  }

  for (auto cit = state.transfers_.cbegin(); cit != state.transfers_.cend(); ++cit)
  {
    const vasyakin::Transfer& tr = cit->second;

    file << "TR " << tr.id_ << ' '
      << escapeSpaces(tr.from_) << ' '
      << escapeSpaces(tr.to_) << ' '
      << escapeSpaces(tr.item_key_) << ' '
      << tr.count_ << ' ' << tr.price_ << ' '
      << tr.departure_.toString() << ' '
      << tr.arrival_.toString() << ' '
      << (tr.active_ ? 1 : 0) << '\n';
  }

  for (auto cit = state.log_.cbegin(); cit != state.log_.cend(); ++cit)
  {
    const vasyakin::LogEntry& log = cit->second;

    file << "LOG " << log.id_ << ' '
      << log.date_.toString() << ' ' << log.details_ << '\n';
  }

  file << "END" << '\n';
  out << "<DATA SAVED TO: " << filename << ">" << '\n';
}

void vasyakin::cmdLoad(
  std::istream& in, std::ostream& out, SystemState& state)
{
  std::string filename;
  if (!(in >> filename))
  {
    throw std::runtime_error("Invalid load args");
  }

  std::ifstream file(filename);
  if (!file.is_open())
  {
    out << "<ERROR: FILE NOT FOUND>" << '\n';
    return;
  }

  state = SystemState();

  std::string token;
  while (file >> token)
  {
    if (token == "#")
    {
      std::string dummy;
      std::getline(file, dummy);
      continue;
    }

    if (token == "META")
    {
      size_t op_cnt = 0;
      size_t tr_cnt = 0;
      std::string date_str;

      file >> op_cnt >> tr_cnt >> date_str;

      state.op_counter_ = op_cnt;
      state.transfer_counter_ = tr_cnt;
      state.current_date_ = Date::fromString(date_str);
    }
    else if (token == "WH")
    {
      std::string name;
      size_t cap = 0, days = 0, used = 0, sent = 0, recv = 0, val = 0;

      file >> name >> cap >> days >> used >> sent >> recv >> val;

      state.warehouses_.insert(name, WarehouseState(name, cap, days));
      vasyakin::WarehouseState& wh = state.warehouses_.at(name);

      wh.used_capacity_ = used;
      wh.items_sent_ = sent;
      wh.items_received_ = recv;
      wh.total_value_ = val;
    }
    else if (token == "IT")
    {
      std::string wh_name, model_raw, color_raw;
      size_t size = 0, count = 0, price = 0;

      file >> wh_name >> model_raw >> color_raw >> size >> count >> price;

      if (state.warehouses_.count(wh_name))
      {
        std::string model = unescapeSpaces(model_raw);
        std::string color = unescapeSpaces(color_raw);

        state.warehouses_.at(wh_name).items_.insert(
          model + "|" + color + "|" + std::to_string(size),
          Item(model, color, size, count, price));
      }
    }
    else if (token == "TR")
    {
      size_t id = 0, count = 0, price = 0, active_flag = 0;
      std::string from_raw, to_raw, key_raw, dep_str, arr_str;

      file >> id >> from_raw >> to_raw >> key_raw >> count >> price >> dep_str >> arr_str >> active_flag;

      std::string from = unescapeSpaces(from_raw);
      std::string to = unescapeSpaces(to_raw);
      std::string key = unescapeSpaces(key_raw);

      state.transfers_.insert(id, Transfer{
        id, from, to, key, count, price,
        Date::fromString(dep_str),
        Date::fromString(arr_str),
        (active_flag == 1)});
    }
    else if (token == "LOG")
    {
      size_t id = 0;
      std::string date_str, details;

      file >> id >> date_str;
      file.ignore(std::numeric_limits< std::streamsize >::max(), ' ');
      std::getline(file, details);

      if (!details.empty() && details.back() == '\r')
      {
        details.pop_back();
      }

      state.log_.insert(id, LogEntry{id, Date::fromString(date_str), details});
    }
    else if (token == "END")
    {
      break;
    }
  }

  state.completeTransfers();
  out << "<DATA LOADED FROM: " << filename << ">" << '\n';
}
