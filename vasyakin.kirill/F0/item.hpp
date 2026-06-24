#ifndef ITEM_HPP
#define ITEM_HPP
#include <string>
#include <cstddef>

namespace vasyakin
{
  class Item
  {
  public:
    Item() noexcept;
    Item(const std::string& model, const std::string& color,
      size_t size, size_t count, size_t price);

    std::string getModel() const noexcept;
    std::string getColor() const noexcept;
    size_t getSize() const noexcept;
    size_t getCount() const noexcept;
    size_t getPrice() const noexcept;

    void addCount(size_t delta) noexcept;
    bool removeCount(size_t delta) noexcept;
    void setCount(size_t new_count) noexcept;

    std::string getKey() const;
    bool isValid() const noexcept;
    size_t getTotalPrice() const noexcept;
    std::string toString() const;

    bool operator>(const Item& other) const noexcept;
    bool operator<(const Item& other) const noexcept;
    bool operator==(const Item& other) const noexcept;
    bool operator!=(const Item& other) const noexcept;
  private:
    std::string model_;
    std::string color_;
    size_t size_;
    size_t count_;
    size_t price_;
  };
}

#endif
