#include "Ad.hpp"

donkeev::Ad::Ad():
  id_(0),
  car_(nullptr),
  price_(0),
  mileage_(0),
  ownerCount_(0),
  isActive_(false)
{}

donkeev::Ad::Ad(Car* car, const std::string& line)
{
  size_t pos = 0;
  
  id_ = std::stoull(nextWord(line, pos));
  car_ = car;
  price_ = std::stoull(nextWord(line, pos));
  mileage_ = std::stoull(nextWord(line, pos));
  ownerCount_ = std::stoull(nextWord(line, pos));
  isActive_ = true;
}

std::string donkeev::Ad::getId() const
{
  return formatId(id_);
}

size_t donkeev::Ad::getIdNumber() const
{
  return id_;
}

const donkeev::Car& donkeev::Ad::getCar() const
{
  return *car_;
}

size_t donkeev::Ad::getPrice() const
{
  return price_;
}

size_t donkeev::Ad::getMileage() const
{
  return mileage_;
}

size_t donkeev::Ad::getOwnerCount() const
{
  return ownerCount_;
}


bool donkeev::Ad::isActive() const
{
  return isActive_;
}

void donkeev::Ad::setId(size_t id)
{
  id_ = id;
}

void donkeev::Ad::setCar(Car* car)
{
  car_ = car;
}

void donkeev::Ad::setPrice(size_t price)
{
  price_ = price;
}

void donkeev::Ad::setMileage(size_t mileage)
{
  mileage_ = mileage;
}

void donkeev::Ad::setOwnerCount(size_t count)
{
  ownerCount_ = count;
}


void donkeev::Ad::setActive(bool active)
{
  isActive_ = active;
}

std::string donkeev::Ad::formatId(size_t id)
{
  std::ostringstream oss;
  oss << std::setw(4) << std::setfill('0') << id;
  return oss.str();
}
