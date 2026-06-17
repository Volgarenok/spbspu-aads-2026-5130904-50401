#include "item.hpp"

vasyakin::Item::Item() noexcept:
  size_(0),
  count_(0),
  price_(0)
{}

vasyakin::Item::Item(const std::string& model, const std::string& color,
  size_t size, size_t count, size_t price):
  model_(model),
  color_(color),
  size_(size),
  count_(count),
  price_(price)
{}

std::string vasyakin::Item::getModel() const noexcept
{
  return model_;
}

std::string vasyakin::Item::getColor() const noexcept
{
  return color_;
}

size_t vasyakin::Item::getSize() const noexcept
{
  return size_;
}

size_t vasyakin::Item::getCount() const noexcept
{
  return count_;
}

size_t vasyakin::Item::getPrice() const noexcept
{
  return price_;
}

void vasyakin::Item::addCount(size_t delta) noexcept
{
  count_ += delta;
}

bool vasyakin::Item::removeCount(size_t delta) noexcept
{
  if (count_ >= delta)
  {
    count_ -= delta;
    return true;
  }

  return false;
}

void vasyakin::Item::setCount(size_t new_count) noexcept
{
  count_ = new_count;
}

std::string vasyakin::Item::getKey() const
{
  return model_ + "|" + color_ + "|" + std::to_string(size_);
}

bool vasyakin::Item::isValid() const noexcept
{
  return !model_.empty() && !color_.empty() && size_ > 0 && price_ > 0;
}

  size_t vasyakin::Item::getTotalPrice() const noexcept
  {
    return count_ * price_;
  }

std::string vasyakin::Item::toString() const
{
  std::string res =  model_ + " " + color_ + " " + std::to_string(size_);
  return res + " :" + " " + std::to_string(count_) +
    " pcs @ " + std::to_string(price_);
}

bool vasyakin::Item::operator>(const Item& other) const noexcept
{
  return getKey() > other.getKey();
}

bool vasyakin::Item::operator<(const Item& other) const noexcept
{
  return getKey() < other.getKey();
}

bool vasyakin::Item::operator==(const Item& other) const noexcept
{
  return getKey() == other.getKey();
}

bool vasyakin::Item::operator!=(const Item& other) const noexcept
{
  return getKey() != other.getKey();
}
