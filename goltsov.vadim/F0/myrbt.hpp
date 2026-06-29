#ifndef MYRBT_HPP
#define MYRBT_HPP
#include <cstddef>
#include <utility>
#include <memory>
#include <mystack.hpp>

namespace goltsov
{
  template< class Key, class Value, class Compare >
  class RBTree;
  template< class Key, class Value >
  class RBTIterator;
  template< class Key, class Value >
  class RBTConstIterator;

  namespace detail
  {
    template< class Key, class Value >
    struct NodeRBT
    {
      std::pair< Key, Value > data;
      NodeRBT< Key, Value >* left;
      NodeRBT< Key, Value >* right;
      NodeRBT< Key, Value >* parent;
      bool is_black;
      size_t height() const noexcept;
      size_t blackHeight() const noexcept;
    };
    template< class Key, class Value >
    RBTIterator< Key, Value > makeRBTIterByPtr(NodeRBT< Key, Value >* ptr);
    template< class Key, class Value >
    RBTConstIterator< Key, Value > makeRBTConstIterByPtr(NodeRBT< Key, Value >* ptr);
    template< class Key, class Value >
    NodeRBT< Key, Value >* falLeft(detail::NodeRBT< Key, Value >* a);
    template< class Key, class Value >
    NodeRBT< Key, Value >* falRight(detail::NodeRBT< Key, Value >* a);
  }

  template< class Key, class Value >
  class RBTIterator
  {
  public:
    RBTIterator();
    RBTIterator(const RBTIterator< Key, Value >&) = default;
    RBTIterator(RBTIterator< Key, Value >&&);
    ~RBTIterator() = default;
    RBTIterator< Key, Value >& operator=(const RBTIterator< Key, Value >&) = default;
    RBTIterator< Key, Value >& operator=(RBTIterator< Key, Value >&&);
    RBTIterator< Key, Value > operator++();
    RBTIterator< Key, Value > operator++(int);
    RBTIterator< Key, Value > operator--();
    RBTIterator< Key, Value > operator--(int);
    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();
    bool operator==(const RBTIterator< Key, Value >&) const noexcept;
    bool operator!=(const RBTIterator< Key, Value >&) const noexcept;
    bool operator==(const RBTConstIterator< Key, Value >&) const noexcept;
    bool operator!=(const RBTConstIterator< Key, Value >&) const noexcept;
    operator RBTConstIterator< Key, Value >() const;
    detail::NodeRBT< Key, Value >* getPtr();
  private:
    template< class K, class V, class C >
    friend class RBTree;
    friend RBTConstIterator< Key, Value >;
    friend RBTIterator< Key, Value > detail::makeRBTIterByPtr< Key, Value >(NodeRBT< Key, Value >* ptr);
    detail::NodeRBT< Key, Value>* ptr_;
    RBTIterator(detail::NodeRBT< Key, Value >*);
    RBTIterator< Key, Value > next() const;
    bool hasNext() const noexcept;
    RBTIterator< Key, Value > prev() const;
    bool hasPrev() const noexcept;
  };

  template< class Key, class Value >
  class RBTConstIterator
  {
  public:
    RBTConstIterator();
    RBTConstIterator(const RBTConstIterator< Key, Value >&) = default;
    RBTConstIterator(RBTConstIterator< Key, Value >&&);
    ~RBTConstIterator() = default;
    RBTConstIterator< Key, Value >& operator=(const RBTConstIterator< Key, Value >&) = default;
    RBTConstIterator< Key, Value >& operator=(RBTConstIterator< Key, Value >&&);
    RBTConstIterator< Key, Value > operator++();
    RBTConstIterator< Key, Value > operator++(int);
    RBTConstIterator< Key, Value > operator--();
    RBTConstIterator< Key, Value > operator--(int);
    const std::pair< Key, Value >& operator*();
    const std::pair< Key, Value >* operator->();
    bool operator==(const RBTIterator< Key, Value >&) const noexcept;
    bool operator!=(const RBTIterator< Key, Value >&) const noexcept;
    bool operator==(const RBTConstIterator< Key, Value >&) const noexcept;
    bool operator!=(const RBTConstIterator< Key, Value >&) const noexcept;
    const detail::NodeRBT< Key, Value >* getPtr();
  private:
    template< class K, class V, class C >
    friend class RBTree;
    friend class RBTIterator< Key, Value >;
    friend RBTConstIterator< Key, Value > detail::makeRBTConstIterByPtr< Key, Value >(NodeRBT< Key, Value >* ptr);
    const detail::NodeRBT< Key, Value>* ptr_;
    RBTConstIterator(const detail::NodeRBT< Key, Value >*);
    RBTConstIterator< Key, Value > next() const;
    bool hasNext() const noexcept;
    RBTConstIterator< Key, Value > prev() const;
    bool hasPrev() const noexcept;
  };

  template< class Key, class Value, class Compare >
  class RBTree
  {
  public:
    RBTree();
    RBTree(const RBTree< Key, Value, Compare >&);
    RBTree(RBTree< Key, Value, Compare >&&);
    ~RBTree() noexcept;
    RBTree< Key, Value, Compare >& operator=(const RBTree< Key, Value, Compare >&);
    RBTree< Key, Value, Compare >& operator=(RBTree< Key, Value, Compare >&&);
    std::pair< RBTIterator< Key, Value >, bool > insert(const std::pair< Key, Value >&);
    std::pair< RBTIterator< Key, Value >, bool > insert(std::pair< Key, Value >&&);
    Value& at(const Key&);
    const Value& at(const Key&) const;
    RBTIterator< Key, Value > find(const Key&);
    RBTConstIterator< Key, Value > find(const Key&) const;
    RBTIterator< Key, Value > getLast() noexcept;
    RBTConstIterator< Key, Value > getLast() const noexcept;
    Value& operator[](const Key&) noexcept;
    size_t erase(const Key&) noexcept;
    void swap(RBTree< Key, Value, Compare >&);
    void clear() noexcept;
    RBTIterator< Key, Value > begin();
    RBTConstIterator< Key, Value > begin() const;
    RBTIterator< Key, Value > end();
    RBTConstIterator< Key, Value > end() const;
    RBTIterator< Key, Value > rotateLeft(RBTConstIterator< Key, Value > it);
    RBTIterator< Key, Value > rotateRight(RBTConstIterator< Key, Value > it);
    RBTIterator< Key, Value > rotateLargeLeft(RBTConstIterator< Key, Value > it);
    RBTIterator< Key, Value > rotateLargeRight(RBTConstIterator< Key, Value > it);
    size_t height(RBTConstIterator< Key, Value > it) const noexcept;
    size_t height() const noexcept;
    size_t blackHeight(RBTConstIterator< Key, Value > it) const noexcept;
    size_t blackHeight() const noexcept;
    size_t size() const noexcept;
    size_t empty() const noexcept;
  private:
    friend class RBTIterator< Key, Value >;
    friend class RBTConstIterator< Key, Value >;
    detail::NodeRBT< Key, Value >* root_;
    size_t size_;
    void makeBalanceAfterPush(detail::NodeRBT< Key, Value >*);
    void makeBalanceAfterDrop(std::tuple< detail::NodeRBT< Key, Value >*, Value, bool >);
  };
}

template< class Key, class Value >
goltsov::detail::NodeRBT< Key, Value >* goltsov::detail::falLeft(detail::NodeRBT< Key, Value >* a)
{
  if (!a)
  {
    return nullptr;
  }
  while (a->left)
  {
    a = a->left;
  }
  return a;
}
template< class Key, class Value >
goltsov::detail::NodeRBT< Key, Value >* goltsov::detail::falRight(detail::NodeRBT< Key, Value >* a)
{
  if (!a)
  {
    return nullptr;
  }
  while (a->right)
  {
    a = a->right;
  }
  return a;
}
template<class Key, class Value>
size_t goltsov::detail::NodeRBT<Key, Value>::height() const noexcept
{
  size_t max_height = 0;
  size_t current_height = 0;
  const NodeRBT<Key, Value>* current = this;
  struct PathNode
  {
    const NodeRBT<Key, Value>* node;
    bool leftvisited;
    bool rightvisited;
  };
  Stack< PathNode > road;
  if (current)
  {
    road.push(PathNode{current, false, false});
    ++current_height;
    ++max_height;
  }
  while (!road.empty())
  {
    if (current->left && (road.empty() || !road.top().leftvisited))
    {
      road.top().leftvisited = true;
      road.push(PathNode{current->left, false, false});
      current = current->left;
      ++current_height;
      max_height = std::max(max_height, current_height);
    }
    else if (current->right && (road.empty() || !road.top().rightvisited))
    {
      road.top().rightvisited = true;
      road.push(PathNode{current->right, false, false});
      current = current->right;
      ++current_height;
      max_height = std::max(max_height, current_height);
    }
    else
    {
      if (!road.empty())
      {
        current = current->parent;
        --current_height;
        while (!road.empty() && road.top().node != current)
        {
          road.pop();
        }
      }
    }
  }
  return max_height;
}
template< class Key, class Value >
size_t goltsov::detail::NodeRBT< Key, Value >::blackHeight() const noexcept
{
  size_t max_black_height = 0;
  size_t current_black_height = 0;
  const NodeRBT<Key, Value>* current = this;
  struct PathNode
  {
    const NodeRBT<Key, Value>* node;
    bool leftvisited;
    bool rightvisited;
  };
  Stack< PathNode > road;
  if (current)
  {
    road.push(PathNode{current, false, false});
    if (current->is_black)
    {
      ++current_black_height;
      ++max_black_height;
    }
  }
  while (!road.empty())
  {
    if (current->left && (road.empty() || !road.top().leftvisited))
    {
      road.top().leftvisited = true;
      road.push(PathNode{current->left, false, false});
      current = current->left;
      if (current->is_black)
      {
        ++current_black_height;
        max_black_height = std::max(max_black_height, current_black_height);
      }
    }
    else if (current->right && (road.empty() || !road.top().rightvisited))
    {
      road.top().rightvisited = true;
      road.push(PathNode{current->right, false, false});
      current = current->right;
      if (current->is_black)
      {
        ++current_black_height;
        max_black_height = std::max(max_black_height, current_black_height);
      }
    }
    else
    {
      if (!road.empty())
      {
        if (current->is_black)
        {
          --current_black_height;
        }
        current = current->parent;
        while (!road.empty() && road.top().node != current)
        {
          road.pop();
        }
      }
    }
  }
  return max_black_height;
}
template< class Key, class Value, class Compare >
goltsov::RBTree< Key, Value, Compare >::RBTree():
  root_(nullptr),
  size_(0)
{}
template< class Key, class Value, class Compare >
goltsov::RBTree< Key, Value, Compare >::~RBTree() noexcept
{
  clear();
}
template< class Key, class Value, class Compare >
goltsov::RBTree< Key, Value, Compare >::RBTree(const RBTree< Key, Value, Compare >& other):
  root_(nullptr),
  size_(0)
{
  detail::NodeRBT< Key, Value >* current = other.root_;
  if (!current)
  {
    return;
  }
  RBTree< Key, Value, Compare > new_tree;
  new_tree.root_ = new detail::NodeRBT< Key, Value >{current->data, nullptr, nullptr, nullptr, current->is_black};
  new_tree.size_ = 1;
  detail::NodeRBT< Key, Value >* new_node = new_tree.root_;
  while (current)
  {
    if (current->left && !new_node->left)
    {
      new_node->left = new detail::NodeRBT< Key, Value >{current->left->data, nullptr, nullptr, new_node,
        current->left->is_black};
      new_node = new_node->left;
      current = current->left;
      ++new_tree.size_;
    }
    else if (current->right && !new_node->right)
    {
      new_node->right = new detail::NodeRBT< Key, Value >{current->right->data, nullptr, nullptr, new_node,
        current->right->is_black};
      new_node = new_node->right;
      current = current->right;
      ++new_tree.size_;
    }
    else
    {
      new_node = new_node->parent;
      current = current->parent;
    }
  }
  swap(new_tree);
}
template< class Key, class Value, class Compare >
goltsov::RBTree< Key, Value, Compare >::RBTree(RBTree< Key, Value, Compare >&& other):
  root_(std::exchange(other.root_, nullptr)),
  size_(std::exchange(other.size_, 0))
{}
template< class Key, class Value, class Compare >
goltsov::RBTree< Key, Value, Compare >&
  goltsov::RBTree< Key, Value, Compare >::operator=(const RBTree< Key, Value, Compare >& other)
{
  if (std::addressof(other) != this)
  {
    RBTree< Key, Value, Compare > new_tree(other);
    swap(new_tree);
  }
  return (*this);
}
template< class Key, class Value, class Compare >
goltsov::RBTree< Key, Value, Compare >&
  goltsov::RBTree< Key, Value, Compare >::operator=(RBTree< Key, Value, Compare >&& other)
{
  if (std::addressof(other) != this)
  {
    swap(other);
  }
  return (*this);
}
template< class Key, class Value, class Compare >
std::pair< goltsov::RBTIterator< Key, Value >, bool >
  goltsov::RBTree< Key, Value, Compare >::insert(const std::pair< Key, Value >& k_v)
{
  Compare comparator;
  detail::NodeRBT< Key, Value >* inserted = nullptr;
  if (!root_)
  {
    inserted = new detail::NodeRBT< Key, Value >{k_v, nullptr, nullptr, nullptr,
      true};
    root_ = inserted;
    ++size_;
  }
  else
  {
    detail::NodeRBT< Key, Value >* current = root_;
    while (true)
    {
      if (comparator(k_v.first, current->data.first))
      {
        if (!current->left)
        {
          inserted = new detail::NodeRBT< Key, Value >{k_v,
            nullptr, nullptr, current, false};
          current->left = inserted;
          ++size_;
          break;
        }
        else
        {
          current = current->left;
        }
      }
      else if (comparator(current->data.first, k_v.first))
      {
        if (!current->right)
        {
          inserted = new detail::NodeRBT< Key, Value >{k_v,
            nullptr, nullptr, current, false};
          current->right = inserted;
          ++size_;
          break;
        }
        else
        {
          current = current->right;
        }
      }
      else
      {
        return {RBTIterator< Key, Value >(current), false};
      }
    }
  }
  makeBalanceAfterPush(inserted);
  return {RBTIterator< Key, Value >(inserted), true};
}
template< class Key, class Value, class Compare >
std::pair< goltsov::RBTIterator< Key, Value >, bool >
  goltsov::RBTree< Key, Value, Compare >::insert(std::pair< Key, Value >&& k_v)
{
  Compare comparator;
  detail::NodeRBT< Key, Value >* inserted = nullptr;
  if (!root_)
  {
    inserted = new detail::NodeRBT< Key, Value >{std::move(k_v), nullptr, nullptr, nullptr,
      true};
    root_ = inserted;
    ++size_;
  }
  else
  {
    detail::NodeRBT< Key, Value >* current = root_;
    while (true)
    {
      if (comparator(k_v.first, current->data.first))
      {
        if (!current->left)
        {
          inserted = new detail::NodeRBT< Key, Value >{std::move(k_v),
            nullptr, nullptr, current, false};
          current->left = inserted;
          ++size_;
          break;
        }
        else
        {
          current = current->left;
        }
      }
      else if (comparator(current->data.first, k_v.first))
      {
        if (!current->right)
        {
          inserted = new detail::NodeRBT< Key, Value >{std::move(k_v),
            nullptr, nullptr, current, false};
          current->right = inserted;
          ++size_;
          break;
        }
        else
        {
          current = current->right;
        }
      }
      else
      {
        return {RBTIterator< Key, Value >(current), false};
      }
    }
  }
  makeBalanceAfterPush(inserted);
  return {RBTIterator< Key, Value >(inserted), true};
}
template< class Key, class Value, class Compare >
Value& goltsov::RBTree< Key, Value, Compare >::at(const Key& k)
{
  return const_cast< Value& >(static_cast< const RBTree< Key, Value, Compare >& >(*this).at(k));
}
template< class Key, class Value, class Compare >
const Value& goltsov::RBTree< Key, Value, Compare >::at(const Key& k) const
{
  Compare comparator;
  detail::NodeRBT< Key, Value >* current = root_;
  while (current)
  {
    if (comparator(k, current->data.first))
    {
      current = current->left;
    }
    else if (comparator(current->data.first, k))
    {
      current = current->right;
    }
    else
    {
      return current->data.second;
    }
  }
  throw std::logic_error("No such key");
}
template< class Key, class Value, class Compare >
goltsov::RBTIterator< Key, Value > goltsov::RBTree< Key, Value, Compare >::find(const Key& k)
{
  Compare comparator;
  detail::NodeRBT< Key, Value >* current = root_;
  while (current)
  {
    if (comparator(k, current->data.first))
    {
      current = current->left;
    }
    else if (comparator(current->data.first, k))
    {
      current = current->right;
    }
    else
    {
      return current;
    }
  }
  return end();
}
template< class Key, class Value, class Compare >
goltsov::RBTConstIterator< Key, Value > goltsov::RBTree< Key, Value, Compare >::find(const Key& k) const
{
  Compare comparator;
  detail::NodeRBT< Key, Value >* current = root_;
  while (current)
  {
    if (comparator(k, current->data.first))
    {
      current = current->left;
    }
    else if (comparator(current->data.first, k))
    {
      current = current->right;
    }
    else
    {
      return current;
    }
  }
  return end();
}
template< class Key, class Value, class Compare >
goltsov::RBTIterator< Key, Value > goltsov::RBTree< Key, Value, Compare >::getLast() noexcept
{
  return RBTIterator< Key, Value >{detail::falRight(root_)};
}
template< class Key, class Value, class Compare >
goltsov::RBTConstIterator< Key, Value > goltsov::RBTree< Key, Value, Compare >::getLast() const noexcept
{
  return RBTConstIterator< Key, Value >{detail::falRight(root_)};
}
template< class Key, class Value, class Compare >
Value& goltsov::RBTree< Key, Value, Compare >::operator[](const Key& k) noexcept
{
  try
  {
    return at(k);
  }
  catch (std::logic_error&)
  {
    return (insert(std::pair< Key, Value >{k, Value{}})).first->second;
  }
}
template< class Key, class Value, class Compare >
size_t goltsov::RBTree< Key, Value, Compare >::erase(const Key& k) noexcept
{
  Compare comparator;
  detail::NodeRBT< Key, Value >* current = root_;
  while (current)
  {
    if (comparator(k, current->data.first))
    {
      current = current->left;
    }
    else if (comparator(current->data.first, k))
    {
      current = current->right;
    }
    else
    {
      if (!current->left && !current->right)
      {
        if (current->parent && current->parent->left == current)
        {
          current->parent->left = nullptr;
        }
        else if (current->parent)
        {
          current->parent->right = nullptr;
        }
        else
        {
          root_ = nullptr;
        }
      }
      else if (current->left && !current->right)
      {
        detail::NodeRBT< Key, Value >* maxLeft = falRight(current->left);
        detail::NodeRBT< Key, Value >* currParent = current->parent;
        detail::NodeRBT< Key, Value >* currLeft = current->left;
        detail::NodeRBT< Key, Value >* maxParent = maxLeft->parent;
        detail::NodeRBT< Key, Value >* maxLeftChild = maxLeft->left;
        std::swap(current->is_black, maxLeft->is_black);
        maxLeft->parent = currParent;
        if (currParent)
        {
          if (currParent->left == current)
          {
            currParent->left = maxLeft;
          }
          else currParent->right = maxLeft;
        }
        else
        {
          root_ = maxLeft;
        }
        maxLeft->right = nullptr;
        if (maxLeft == currLeft)
        {
          maxLeft->left = current;
          current->parent = maxLeft;
        }
        else
        {
          maxLeft->left = currLeft;
          if (currLeft)
          {
            currLeft->parent = maxLeft;
          }
          current->parent = maxParent;
          if (maxParent)
          {
            if (maxParent->left == maxLeft)
            {
              maxParent->left = current;
            }
            else maxParent->right = current;
          }
        }
        current->right = nullptr;
        current->left = maxLeftChild;
        if (maxLeftChild)
        {
          maxLeftChild->parent = current;
        }
        if (current->parent && current->parent->left == current)
        {
          current->parent->left = current->left;
          if (current->left)
          {
            current->left->parent = current->parent;
          }
        }
        else if (current->parent)
        {
          current->parent->right = current->left;
          if (current->left)
          {
            current->left->parent = current->parent;
          }
        }
        else
        {
          root_ = current->left;
          if (current->left)
          {
            current->left->parent = nullptr;
          }
        }
      }
      else
      {
        detail::NodeRBT< Key, Value >* minRight = falLeft(current->right);
        detail::NodeRBT< Key, Value >* currParent = current->parent;
        detail::NodeRBT< Key, Value >* currLeft = current->left;
        detail::NodeRBT< Key, Value >* currRight = current->right;
        detail::NodeRBT< Key, Value >* minParent = minRight->parent;
        detail::NodeRBT< Key, Value >* minRightChild = minRight->right;
        std::swap(current->is_black, minRight->is_black);
        minRight->parent = currParent;
        if (currParent)
        {
          if (currParent->left == current) currParent->left = minRight;
          else currParent->right = minRight;
        }
        else
        {
          root_ = minRight;
        }
        minRight->left = currLeft;
        if (currLeft)
        {
          currLeft->parent = minRight;
        }
        if (minRight == currRight)
        {
          minRight->right = current;
          current->parent = minRight;
        }
        else
        {
          minRight->right = currRight;
          if (currRight)
          {
            currRight->parent = minRight;
          }
          current->parent = minParent;
          if (minParent)
          {
            if (minParent->left == minRight) minParent->left = current;
            else minParent->right = current;
          }
        }
        current->left = nullptr;
        current->right = minRightChild;
        if (minRightChild) minRightChild->parent = current;
        if (current->parent && current->parent->left == current)
        {
          current->parent->left = current->right;
          if (current->right)
          {
            current->right->parent = current->parent;
          }
        }
        else if (current->parent)
        {
          current->parent->right = current->right;
          if (current->right)
          {
            current->right->parent = current->parent;
          }
        }
        else
        {
          root_ = current->right;
          if (current->right)
          {
            current->right->parent = nullptr;
          }
        }
      }
      detail::NodeRBT< Key, Value >* res_ptr = current->parent;
      Value res_val = current->data.second;
      bool res_is_black = current->is_black;
      delete current;
      size_ -= 1;
      makeBalanceAfterDrop(std::make_tuple(res_ptr, res_val, res_is_black));
      return 1;
    }
  }
  return 0;
}
template< class Key, class Value, class Compare >
void goltsov::RBTree< Key, Value, Compare >::swap(RBTree< Key, Value, Compare >& other)
{
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
}
template< class Key, class Value, class Compare >
void goltsov::RBTree< Key, Value, Compare >::clear() noexcept
{
  detail::NodeRBT< Key, Value >* current = root_;
  while (current)
  {
    if (!current->left && !current->right)
    {
      if (!current->parent)
      {
        delete current;
        root_ = nullptr;
        current = nullptr;
      }
      else
      {
        if (current == current->parent->left)
        {
          current = current->parent;
          delete current->left;
          current->left = nullptr;
        }
        else
        {
          current = current->parent;
          delete current->right;
          current->right = nullptr;
        }
      }
    }
    else if (current->left)
    {
      current = current->left;
    }
    else if (current->right)
    {
      current = current->right;
    }
  }
  size_ = 0;
}
template< class Key, class Value, class Compare >
goltsov::RBTIterator< Key, Value > goltsov::RBTree< Key, Value, Compare >::begin()
{
  return RBTIterator< Key, Value >(detail::falLeft(root_));
}
template< class Key, class Value, class Compare >
goltsov::RBTConstIterator< Key, Value > goltsov::RBTree< Key, Value, Compare >::begin() const
{
  return RBTConstIterator< Key, Value >(detail::falLeft(root_));
}
template< class Key, class Value, class Compare >
goltsov::RBTIterator< Key, Value > goltsov::RBTree< Key, Value, Compare >::end()
{
  return RBTIterator< Key, Value >(nullptr);
}
template< class Key, class Value, class Compare >
goltsov::RBTConstIterator< Key, Value > goltsov::RBTree< Key, Value, Compare >::end() const
{
  return RBTConstIterator< Key, Value >(nullptr);
}
template< class Key, class Value, class Compare >
goltsov::RBTIterator< Key, Value > goltsov::RBTree< Key, Value, Compare >::rotateLeft(RBTConstIterator< Key, Value > it)
{
  detail::NodeRBT< Key, Value >* current = const_cast< detail::NodeRBT< Key, Value >* >(it.ptr_);
  if (!current)
  {
    throw std::logic_error("Can not do rotate. Iterator is empty");
  }
  if (!current->parent)
  {
    throw std::logic_error("Can not do rotate. No parent");
  }
  if (current == current->parent->left)
  {
    throw std::logic_error("Can not do left rotate.");
  }
  detail::NodeRBT< Key, Value >* parent = current->parent;
  detail::NodeRBT< Key, Value >* grandparent = current->parent->parent;
  detail::NodeRBT< Key, Value >* left = current->left;
  parent->right = left;
  if (left)
  {
    left->parent = parent;
  }
  current->left = parent;
  parent->parent = current;
  current->parent = grandparent;
  if (grandparent)
  {
    if (grandparent->left == parent)
    {
      grandparent->left = current;
    }
    else
    {
      grandparent->right = current;
    }
  }
  else
  {
    root_ = current;
  }
  return RBTIterator< Key, Value >(current->right);
}
template< class Key, class Value, class Compare >
goltsov::RBTIterator< Key, Value >
  goltsov::RBTree< Key, Value, Compare >::rotateRight(RBTConstIterator< Key, Value > it)
{
  detail::NodeRBT< Key, Value >* current = const_cast< detail::NodeRBT< Key, Value >* >(it.ptr_);
  if (!current)
  {
    throw std::logic_error("Can not do rotate. Iterator is empty");
  }
  if (!current->parent)
  {
    throw std::logic_error("Can not do rotate. No parent");
  }
  if (current == current->parent->right)
  {
    throw std::logic_error("Can not do right rotate.");
  }
  detail::NodeRBT< Key, Value >* parent = current->parent;
  detail::NodeRBT< Key, Value >* grandparent = current->parent->parent;
  detail::NodeRBT< Key, Value >* right = current->right;
  parent->left = right;
  if (right)
  {
    right->parent = parent;
  }
  current->right = parent;
  parent->parent = current;
  current->parent = grandparent;
  if (grandparent)
  {
    if (grandparent->left == parent)
    {
      grandparent->left = current;
    }
    else
    {
      grandparent->right = current;
    }
  }
  else
  {
    root_ = current;
  }
  return RBTIterator< Key, Value >(current->left);
}
template< class Key, class Value, class Compare >
goltsov::RBTIterator< Key, Value >
  goltsov::RBTree< Key, Value, Compare >::rotateLargeLeft(RBTConstIterator< Key, Value > it)
{
  detail::NodeRBT< Key, Value >* current = const_cast< detail::NodeRBT< Key, Value >* >(it.ptr_);
  if (!current->parent || !current->parent->parent)
  {
    throw std::logic_error("Can not do rotate. No parent");
  }
  rotateRight(it);
  rotateLeft(it);
  return RBTIterator< Key, Value >(it.ptr_->right->left);
}
template< class Key, class Value, class Compare >
goltsov::RBTIterator< Key, Value >
  goltsov::RBTree< Key, Value, Compare >::rotateLargeRight(RBTConstIterator< Key, Value > it)
{
  detail::NodeRBT< Key, Value >* current = const_cast< detail::NodeRBT< Key, Value >* >(it.ptr_);
  if (!current->parent || !current->parent->parent)
  {
    throw std::logic_error("Can not do rotate. No parent");
  }
  rotateLeft(it);
  rotateRight(it);
  return RBTIterator< Key, Value >(it.ptr_->left->right);
}
template< class Key, class Value, class Compare >
size_t goltsov::RBTree< Key, Value, Compare >::height(RBTConstIterator< Key, Value > it) const noexcept
{
  if (!it.ptr_)
  {
    return 0;
  }
  return it.ptr_->height();
}
template< class Key, class Value, class Compare >
size_t goltsov::RBTree< Key, Value, Compare >::height() const noexcept
{
  if (!root_)
  {
    return 0;
  }
  return root_->height();
}
template< class Key, class Value, class Compare >
size_t goltsov::RBTree< Key, Value, Compare >::blackHeight(RBTConstIterator< Key, Value > it) const noexcept
{
  if (!it.ptr_)
  {
    return 0;
  }
  return it.ptr_->blackHeight();
}
template< class Key, class Value, class Compare >
size_t goltsov::RBTree< Key, Value, Compare >::blackHeight() const noexcept
{
  if (!root_)
  {
    return 0;
  }
  return root_->blackHeight();
}
template< class Key, class Value, class Compare >
size_t goltsov::RBTree< Key, Value, Compare >::size() const noexcept
{
  return size_;
}
template< class Key, class Value, class Compare >
size_t goltsov::RBTree< Key, Value, Compare >::empty() const noexcept
{
  return size_ == 0;
}
template< class Key, class Value >
goltsov::RBTIterator< Key, Value >::RBTIterator():
  ptr_(nullptr)
{}
template< class Key, class Value >
goltsov::RBTIterator< Key, Value >::RBTIterator(RBTIterator< Key, Value >&& other):
  ptr_(other.ptr_)
{
  other.ptr_ = nullptr;
}
template< class Key, class Value >
goltsov::RBTIterator< Key, Value >& goltsov::RBTIterator< Key, Value >::operator=(RBTIterator< Key, Value >&& other)
{
  ptr_ = other.ptr_;
  other.ptr_ = nullptr;
  return (*this);
}
template< class Key, class Value >
goltsov::RBTIterator< Key, Value >::RBTIterator(detail::NodeRBT< Key, Value >* ptr):
  ptr_(ptr)
{}
template< class Key, class Value >
goltsov::RBTIterator< Key, Value > goltsov::RBTIterator< Key, Value >::next() const
{
  detail::NodeRBT< Key, Value >* current = ptr_;
  if (!current)
  {
    throw std::logic_error("No next");
  }
  if (current->right)
  {
    return RBTIterator< Key, Value >(detail::falLeft(current->right));
  }
  detail::NodeRBT< Key, Value >* previos = current;
  while (current)
  {
    if (current->left == previos)
    {
      break;
    }
    previos = current;
    current = current->parent;
  }
  return RBTIterator< Key, Value >(current);
}
template< class Key, class Value >
bool goltsov::RBTIterator< Key, Value >::hasNext() const noexcept
{
  return ptr_;
}
template< class Key, class Value >
goltsov::RBTIterator< Key, Value > goltsov::RBTIterator< Key, Value >::prev() const
{
  detail::NodeRBT< Key, Value >* current = ptr_;
  if (!current)
  {
    throw std::logic_error("No prev");
  }
  if (current->left)
  {
    return RBTIterator< Key, Value >(detail::falRight(current->left));
  }
  detail::NodeRBT< Key, Value >* previos = current;
  while (current)
  {
    if (current->right == previos)
    {
      break;
    }
    previos = current;
    current = current->parent;
  }
  if (!current)
  {
    throw std::logic_error("No prev");
  }
  return RBTIterator< Key, Value >(current);
}
template< class Key, class Value >
bool goltsov::RBTIterator< Key, Value >::hasPrev() const noexcept
{
  try
  {
    prev();
    return true;
  }
  catch (...)
  {
    return false;
  }
}
template< class Key, class Value >
goltsov::RBTIterator< Key, Value > goltsov::RBTIterator< Key, Value >::operator++()
{
  (*this) = next();
  return (*this);
}
template< class Key, class Value >
goltsov::RBTIterator< Key, Value > goltsov::RBTIterator< Key, Value >::operator++(int)
{
  RBTIterator< Key, Value > temp = (*this);
  (*this) = next();
  return temp;
}
template< class Key, class Value >
goltsov::RBTIterator< Key, Value > goltsov::RBTIterator< Key, Value >::operator--()
{
  (*this) = prev();
  return (*this);
}
template< class Key, class Value >
goltsov::RBTIterator< Key, Value > goltsov::RBTIterator< Key, Value >::operator--(int)
{
  RBTIterator< Key, Value > temp = (*this);
  (*this) = prev();
  return temp;
}
template< class Key, class Value >
std::pair< Key, Value >& goltsov::RBTIterator< Key, Value >::operator*()
{
  return ptr_->data;
}
template< class Key, class Value >
std::pair< Key, Value >* goltsov::RBTIterator< Key, Value >::operator->()
{
  return std::addressof(ptr_->data);
}
template< class Key, class Value >
goltsov::RBTIterator< Key, Value >::operator goltsov::RBTConstIterator< Key, Value >() const
{
  return RBTConstIterator< Key, Value >(ptr_);
}
template< class Key, class Value >
goltsov::RBTConstIterator< Key, Value >::RBTConstIterator():
  ptr_(nullptr)
{}
template< class Key, class Value >
goltsov::RBTConstIterator< Key, Value >::RBTConstIterator(RBTConstIterator< Key, Value >&& other):
  ptr_(other.ptr_)
{
  other.ptr_ = nullptr;
}
template< class Key, class Value >
goltsov::RBTConstIterator< Key, Value >&
  goltsov::RBTConstIterator< Key, Value >::operator=(RBTConstIterator< Key, Value >&& other)
{
  ptr_ = other.ptr_;
  other.ptr_ = nullptr;
  return (*this);
}
template< class Key, class Value >
goltsov::RBTConstIterator< Key, Value >::RBTConstIterator(const detail::NodeRBT< Key, Value >* ptr)
{
  ptr_ = ptr;
}
template< class Key, class Value >
goltsov::RBTConstIterator< Key, Value > goltsov::RBTConstIterator< Key, Value >::next() const
{
  detail::NodeRBT< Key, Value >* current = const_cast< detail::NodeRBT< Key, Value >* >(ptr_);
  if (!current)
  {
    throw std::logic_error("No next");
  }
  if (current->right)
  {
    return RBTConstIterator< Key, Value >(detail::falLeft(current->right));
  }
  detail::NodeRBT< Key, Value >* previos = current;
  while (current)
  {
    if (current->left == previos)
    {
      break;
    }
    previos = current;
    current = current->parent;
  }
  return RBTConstIterator< Key, Value >(current);
}
template< class Key, class Value >
bool goltsov::RBTConstIterator< Key, Value >::hasNext() const noexcept
{
  return ptr_;
}
template< class Key, class Value >
goltsov::RBTConstIterator< Key, Value > goltsov::RBTConstIterator< Key, Value >::prev() const
{
  detail::NodeRBT< Key, Value >* current = const_cast< detail::NodeRBT< Key, Value >* >(ptr_);
  if (!current)
  {
    throw std::logic_error("No prev");
  }
  if (current->left)
  {
    return RBTConstIterator< Key, Value >(detail::falRight(current->left));
  }
  detail::NodeRBT< Key, Value >* previos = current;
  while (current)
  {
    if (current->right == previos)
    {
      break;
    }
    previos = current;
    current = current->parent;
  }
  return RBTConstIterator< Key, Value >(current);
}
template< class Key, class Value >
bool goltsov::RBTConstIterator< Key, Value >::hasPrev() const noexcept
{
  return prev().ptr_;
}
template< class Key, class Value >
goltsov::RBTConstIterator< Key, Value > goltsov::RBTConstIterator< Key, Value >::operator++()
{
  (*this) = next();
  return (*this);
}
template< class Key, class Value >
goltsov::RBTConstIterator< Key, Value > goltsov::RBTConstIterator< Key, Value >::operator++(int)
{
  RBTConstIterator< Key, Value > temp = (*this);
  (*this) = next();
  return temp;
}
template< class Key, class Value >
goltsov::RBTConstIterator< Key, Value > goltsov::RBTConstIterator< Key, Value >::operator--()
{
  (*this) = prev();
  return (*this);
}
template< class Key, class Value >
goltsov::RBTConstIterator< Key, Value > goltsov::RBTConstIterator< Key, Value >::operator--(int)
{
  RBTConstIterator< Key, Value > temp = (*this);
  (*this) = prev();
  return temp;
}
template< class Key, class Value >
const std::pair< Key, Value >& goltsov::RBTConstIterator< Key, Value >::operator*()
{
  return ptr_->data;
}
template< class Key, class Value >
const std::pair< Key, Value >* goltsov::RBTConstIterator< Key, Value >::operator->()
{
  return std::addressof(ptr_->data);
}
template< class Key, class Value >
bool goltsov::RBTIterator< Key, Value >::operator==(const RBTIterator< Key, Value >& other) const noexcept
{
  return ptr_ == other.ptr_;
}
template< class Key, class Value >
bool goltsov::RBTIterator< Key, Value >::operator!=(const RBTIterator< Key, Value >& other) const noexcept
{
  return !((*this) == other);
}
template< class Key, class Value >
bool goltsov::RBTIterator< Key, Value >::operator==(const RBTConstIterator< Key, Value >& other) const noexcept
{
  return ptr_ == other.ptr_;
}
template< class Key, class Value >
bool goltsov::RBTIterator< Key, Value >::operator!=(const RBTConstIterator< Key, Value >& other) const noexcept
{
  return !((*this) == other);
}
template< class Key, class Value >
bool goltsov::RBTConstIterator< Key, Value >::operator==(const RBTIterator< Key, Value >& other) const noexcept
{
  return ptr_ == other.ptr_;
}
template< class Key, class Value >
bool goltsov::RBTConstIterator< Key, Value >::operator!=(const RBTIterator< Key, Value >& other) const noexcept
{
  return !((*this) == other);
}
template< class Key, class Value >
bool goltsov::RBTConstIterator< Key, Value >::operator==(const RBTConstIterator< Key, Value >& other) const noexcept
{
  return ptr_ == other.ptr_;
}
template< class Key, class Value >
bool goltsov::RBTConstIterator< Key, Value >::operator!=(const RBTConstIterator< Key, Value >& other) const noexcept
{
  return !((*this) == other);
}
template< class Key, class Value >
goltsov::detail::NodeRBT< Key, Value >* goltsov::RBTIterator< Key, Value >::getPtr()
{
  return ptr_;
}
template< class Key, class Value >
const goltsov::detail::NodeRBT< Key, Value >* goltsov::RBTConstIterator< Key, Value >::getPtr()
{
  return ptr_;
}
template< class Key, class Value >
goltsov::RBTIterator< Key, Value > goltsov::detail::makeRBTIterByPtr(NodeRBT< Key, Value >* ptr)
{
  return RBTIterator< Key, Value >(ptr);
}
template< class Key, class Value >
goltsov::RBTConstIterator< Key, Value > goltsov::detail::makeRBTConstIterByPtr(NodeRBT< Key, Value >* ptr)
{
  return RBTConstIterator< Key, Value >(ptr);
}
template< class Key, class Value, class Compare >
void goltsov::RBTree< Key, Value, Compare >::makeBalanceAfterPush(detail::NodeRBT< Key, Value >* pushed)
{
  detail::NodeRBT< Key, Value >* X = pushed;
  detail::NodeRBT< Key, Value >* P = X->parent;
  detail::NodeRBT< Key, Value >* G = P ? P->parent : nullptr;
  detail::NodeRBT< Key, Value >* U = G ? (G->left == P ? G->right : G->left) : nullptr;
  if (P)
  {
    if (!P->is_black && !G)
    {
      P->is_black = true;
    }
    else if (!P->is_black && G && G->is_black && U && !U->is_black)
    {
      P->is_black = true;
      U->is_black = true;
      G->is_black = false;
      makeBalanceAfterPush(G);
    }
    else if (!P->is_black && G && G->is_black && (!U || U->is_black))
    {
      if ((P->left == X) != (G->left == P))
      {
        if (P->left == X)
        {
          rotateRight(RBTConstIterator< Key, Value >{X});
        }
        else
        {
          rotateLeft(RBTConstIterator< Key, Value >{X});
        }
        std::swap(X, P);
      }
      if (P->left == X)
      {
        rotateRight(RBTConstIterator< Key, Value >{P});
      }
      else
      {
        rotateLeft(RBTConstIterator< Key, Value >{P});
      }
      G->is_black = false;
      P->is_black = true;
    }
  }
}
template< class Key, class Value, class Compare >
void goltsov::RBTree< Key, Value, Compare >::makeBalanceAfterDrop(std::tuple< detail::NodeRBT< Key, Value >*, Value, bool >
  dropedParent)
{
  detail::NodeRBT< Key, Value >* P = std::get< 0 >(dropedParent);
  if (!P)
  {
    return;
  }
  if (std::get< 2 >(dropedParent))
  {
    if (blackHeight(RBTConstIterator< Key, Value >(P->left)) < blackHeight(
      RBTConstIterator< Key, Value >(P->right)) && P->left && !P->left->is_black)
    {
      P->left->is_black = true;
    }
    else if (blackHeight(RBTConstIterator< Key, Value >(P->left)) > blackHeight(
      RBTConstIterator< Key, Value >(P->right)) && P->right && !P->right->is_black)
    {
      P->right->is_black = true;
    }
    else
    {
      detail::NodeRBT< Key, Value >* B = (blackHeight(RBTConstIterator< Key, Value >(P->left)) <
        blackHeight(RBTConstIterator< Key, Value >(P->right))) ? P->right : P->left;
      if (B)
      {
        if (B->is_black && ((P->left == B && B->left && !B->left->is_black) || (P->right == B && B->right
          && !B->right->is_black)))
        {
          if (P->left == B)
          {
            rotateRight(RBTConstIterator< Key, Value >{B});
            B->is_black = P->is_black;
            B->left->is_black = true;
            P->is_black = true;
          }
          else
          {
            rotateLeft(RBTConstIterator< Key, Value >{B});
            B->is_black = P->is_black;
            B->right->is_black = true;
            P->is_black = true;
          }
        }
        else if (B->is_black && ((P->left == B && B->right && !B->right->is_black) || (P->right == B
          && B->left && !B->left->is_black)))
        {
          if (P->left == B)
          {
            rotateLeft(RBTConstIterator< Key, Value >{B->right});
            B->is_black = false;
            B->parent->is_black = true;
          }
          else
          {
            rotateRight(RBTConstIterator< Key, Value >{B->left});
            B->is_black = false;
            B->parent->is_black = true;
          }
        }
        else if (!P->is_black && B->is_black && (!B->left || B->left->is_black) && (!B->right
          || B->right->is_black))
        {
          B->is_black = false;
          P->is_black = true;
        }
        else if (P->is_black && !B->is_black && (!B->left || B->left->is_black) && (!B->right
          || B->right->is_black))
        {
          if (P->left == B)
          {
            rotateRight(RBTConstIterator< Key, Value >{B});
            B->is_black = true;
            P->is_black = false;
            makeBalanceAfterDrop(dropedParent);
          }
          else
          {
            rotateLeft(RBTConstIterator< Key, Value >{B});
            B->is_black = true;
            P->is_black = false;
            makeBalanceAfterDrop(dropedParent);
          }
        }
        else if (P->is_black && B->is_black && (!B->left || B->left->is_black)
          && (!B->right || B->right->is_black))
        {
          B->is_black = false;
          makeBalanceAfterDrop({P->parent, P->data.second, true});
        }
      }
    }
  }
}

#endif
