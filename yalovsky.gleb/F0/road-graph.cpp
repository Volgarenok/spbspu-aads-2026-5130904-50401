#include "road-graph.hpp"
#include <stdexcept>

bool yalovsky::isValidName(const std::string& name)
{
  return !name.empty() && name.find_first_of(" \t\n\r\f\v") == std::string::npos;
}

bool yalovsky::RoadGraph::hasCity(
    const std::string& name) const
{
  return cities_.contains(name);
}

bool yalovsky::RoadGraph::hasRoad(
    const std::string& from,
    const std::string& to) const
{
  if (!cities_.contains(from))
  {
    return false;
  }

  return cities_.at(from).contains(to);
}

void yalovsky::RoadGraph::addCity(const std::string& name)
{
  if (!isValidName(name))
  {
    throw std::invalid_argument("Invalid city name");
  }
  if (!cities_.insert(name, Roads{}))
  {
    throw std::invalid_argument("City already exists");
  }
}

void yalovsky::RoadGraph::removeCity(const std::string& name)
{
  if (!hasCity(name))
  {
    throw std::out_of_range("City not found");
  }

  cities_.forEach([&name](const std::string&, Roads& roads)
  {
    roads.erase(name);
  });
  cities_.erase(name);
}

void yalovsky::RoadGraph::addRoad(const std::string& from, const std::string& to,
    Distance distance)
{
  if (from == to || distance == 0)
  {
    throw std::invalid_argument("Invalid road");
  }

  Roads& forward = cities_.at(from);
  Roads& backward = cities_.at(to);

  if (forward.contains(to))
  {
    throw std::invalid_argument("Road already exists");
  }

  forward.insert(to, distance);
  try
  {
    backward.insert(from, distance);
  }
  catch (...)
  {
    forward.erase(to);
    throw;
  }
}

void yalovsky::RoadGraph::removeRoad(const std::string& from, const std::string& to)
{
  Roads& forward = cities_.at(from);
  Roads& backward = cities_.at(to);

  if (!forward.contains(to))
  {
    throw std::out_of_range("Road not found");
  }

  forward.erase(to);
  backward.erase(from);
}

yalovsky::Distance yalovsky::RoadGraph::getRoadWeight(const std::string& from,
    const std::string& to) const
{
  return cities_.at(from).at(to);
}

const yalovsky::RoadGraph::Roads&
yalovsky::RoadGraph::getRoads(const std::string& city) const
{
  return cities_.at(city);
}

const yalovsky::RoadGraph::Cities& yalovsky::RoadGraph::getCities() const noexcept
{
  return cities_;
}

std::size_t yalovsky::RoadGraph::getRoadCount() const
{
  std::size_t count = 0;

  cities_.forEach([&count](const std::string& from, const Roads& roads)
  {
    roads.forEach([&count, &from](const std::string& to, const Distance&)
    {
      if (from < to)
      {
        ++count;
      }
    });
  });

  return count;
}
