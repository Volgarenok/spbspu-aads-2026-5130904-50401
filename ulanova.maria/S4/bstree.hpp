#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <memory>

namespace ulanova
{
  namespace detail
  {
    struct NodeBase
    {
      NodeBase* parent;
      NodeBase* left;
      NodeBase* right;
      size_t height;

      NodeBase();
      NodeBase(NodeBase* parent_node, NodeBase* left_node, NodeBase* right_node);
      virtual ~NodeBase();
    };

    template< class Key, class Value >
    struct Node: NodeBase
    {
      std::pair< const Key, Value > data;

      Node( const Key& key, const Value& value, NodeBase* parent_node, NodeBase* fake_leaf);
    };
  };

  template< class Key, class Value >
  class BSTreeConstIterator;

  template< class Key, class Value >
  class BSTreeIterator;

  template< class Key, class Value, class Compare >
  class BSTree
  {
  public:
    BSTree();
    ~BSTree();

    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear() noexcept;

    void push(const Key& key, const Value& value);
    Value drop(Key key);
    Value get(Key key) const;

    using const_iterator = BSTreeConstIterator< Key, Value >;

    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    using iterator = BSTreeIterator< Key, Value >;

    iterator begin() noexcept;
    iterator end() noexcept;

    size_t height() const noexcept;
    size_t height(const_iterator it) const noexcept;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    BSTree(const BSTree & other);
    BSTree & operator=(const BSTree & other);

  private:
    detail::NodeBase* fake_root_;
    detail::NodeBase* fake_leaf_;
    size_t size_;
    Compare compare_;

    void create_fakes();
    void clear_from(detail::NodeBase* node) noexcept;

    using node_t = detail::Node< Key, Value >;

    node_t * as_node(detail::NodeBase* node) const noexcept;
    void update_height(detail::NodeBase* node) noexcept;
    size_t get_height(detail::NodeBase* node) const noexcept;

    detail::NodeBase * find_node(const Key & key) const noexcept;

    detail::NodeBase * min_node(detail::NodeBase * node) const noexcept;
    detail::NodeBase * next_node(detail::NodeBase * node) const noexcept;

    friend class BSTreeConstIterator< Key, Value >;
    friend class BSTreeIterator< Key, Value >;
    void replace_node(detail::NodeBase * old_node, detail::NodeBase * new_node) noexcept;

    void rotate_left_node(detail::NodeBase * node);
    void rotate_right_node(detail::NodeBase * node);

    void copy_from(detail::NodeBase * node, detail::NodeBase * other_fake_leaf);
    void swap(BSTree & other) noexcept;
  };

  template< class Key, class Value >
  class BSTreeConstIterator
  {
  public:
    using value_type = std::pair< const Key, Value >;
    using reference = const value_type &;
    using pointer = const value_type *;

    BSTreeConstIterator();

    reference operator*() const;
    pointer operator->() const;

    BSTreeConstIterator & operator++();
    BSTreeConstIterator operator++(int);

    bool operator==(const BSTreeConstIterator & rhs) const noexcept;
    bool operator!=(const BSTreeConstIterator & rhs) const noexcept;

  private:
    detail::NodeBase * node_;
    detail::NodeBase * fake_leaf_;

    BSTreeConstIterator(detail::NodeBase * node, detail::NodeBase * fake_leaf);

    template< class K, class V, class C >
    friend class BSTree;
  };

  template< class Key, class Value >
  class BSTreeIterator
  {
  public:
    using value_type = std::pair< const Key, Value >;
    using reference = value_type &;
    using pointer = value_type *;

    BSTreeIterator();

    reference operator*() const;
    pointer operator->() const;

    BSTreeIterator & operator++();
    BSTreeIterator operator++(int);

    bool operator==(const BSTreeIterator & rhs) const noexcept;
    bool operator!=(const BSTreeIterator & rhs) const noexcept;

  private:
    detail::NodeBase * node_;
    detail::NodeBase * fake_leaf_;

    BSTreeIterator(detail::NodeBase * node, detail::NodeBase * fake_leaf);

    template< class K, class V, class C >
    friend class BSTree;
  };
}

template< class Key, class Value >
ulanova::detail::Node< Key, Value >::Node(
  const Key& key,
  const Value& value,
  ulanova::detail::NodeBase* parent_node,
  ulanova::detail::NodeBase* fake_leaf
):
  ulanova::detail::NodeBase(parent_node, fake_leaf, fake_leaf),
  data(key, value)
{
  height = 1;
}

template< class Key, class Value, class Compare >
ulanova::BSTree< Key, Value, Compare >::BSTree():
  fake_root_(nullptr),
  fake_leaf_(nullptr),
  size_(0),
  compare_(Compare())
{
  create_fakes();
}

template< class Key, class Value, class Compare >
ulanova::BSTree< Key, Value, Compare >::~BSTree()
{
  if (fake_root_ != nullptr)
  {
    clear(),
    delete fake_root_;
    delete fake_leaf_;
  }
}

template< class Key, class Value, class Compare >
bool ulanova::BSTree< Key, Value, Compare >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Compare >
std::size_t ulanova::BSTree< Key, Value, Compare >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Compare >
void ulanova::BSTree< Key, Value, Compare >::clear() noexcept
{
  clear_from(fake_root_->left);
  fake_root_->left = fake_leaf_;
  fake_root_->right = fake_leaf_;
  size_ = 0;
}

template< class Key, class Value, class Compare >
void ulanova::BSTree< Key, Value, Compare >::create_fakes()
{
  fake_leaf_ = new detail::NodeBase();
  try
  {
    fake_root_ = new detail::NodeBase(nullptr, fake_leaf_, fake_leaf_);
  }
  catch (...)
  {
    delete fake_leaf_;
    fake_leaf_ = nullptr;
    throw;
  }
}

template< class Key, class Value, class Compare >
void ulanova::BSTree< Key, Value, Compare >::clear_from(detail::NodeBase * node) noexcept
{
  if (node == fake_leaf_)
  {
    return;
  }

  clear_from(node->left);
  clear_from(node->right);
  delete node;
}

template< class Key, class Value, class Compare >
void ulanova::BSTree< Key, Value, Compare >::push(const Key & key, const Value & value)
{
  detail::NodeBase * parent = fake_root_;
  detail::NodeBase * current = fake_root_->left;

  while (current != fake_leaf_)
  {
    node_t * current_node = as_node(current);
    parent = current;

    if (compare_(key, current_node->data.first))
    {
      current = current->left;
    }
    else if (compare_(current_node->data.first, key))
    {
      current = current->right;
    }
    else
    {
      current_node->data.second = value;
      return;
    }
  }

  detail::NodeBase * new_node = new node_t(key, value, parent, fake_leaf_);

  if (parent == fake_root_)
  {
    fake_root_->left = new_node;
  }
  else
  {
    node_t * parent_node = as_node(parent);
    if (compare_(key, parent_node->data.first))
    {
      parent->left = new_node;
    }
    else
    {
      parent->right = new_node;
    }
  }

  ++size_;

  while (parent != nullptr)
  {
    update_height(parent);
    parent = parent->parent;
  }
}

template< class Key, class Value, class Compare >
typename ulanova::BSTree< Key, Value, Compare >::node_t *
ulanova::BSTree< Key, Value, Compare >::as_node(detail::NodeBase * node) const noexcept
{
  return static_cast< node_t * >(node);
}

template< class Key, class Value, class Compare >
std::size_t ulanova::BSTree< Key, Value, Compare >::get_height(detail::NodeBase * node) const noexcept
{
  if (node == fake_leaf_)
  {
    return 0;
  }
  return node->height;
}

template< class Key, class Value, class Compare >
void ulanova::BSTree< Key, Value, Compare >::update_height(detail::NodeBase * node) noexcept
{
  const std::size_t left_height = get_height(node->left);
  const std::size_t right_height = get_height(node->right);

  if (left_height > right_height)
  {
    node->height = left_height + 1;
  }
  else
  {
    node->height = right_height + 1;
  }
}

template< class Key, class Value, class Compare >
ulanova::detail::NodeBase *
ulanova::BSTree< Key, Value, Compare >::find_node(const Key & key) const noexcept
{
  detail::NodeBase * current = fake_root_->left;

  while (current != fake_leaf_)
  {
    node_t * current_node = as_node(current);

    if (compare_(key, current_node->data.first))
    {
      current = current->left;
    }
    else if (compare_(current_node->data.first, key))
    {
      current = current->right;
    }
    else
    {
      return current;
    }
  }

  return fake_leaf_;
}

template< class Key, class Value >
ulanova::BSTreeConstIterator< Key, Value >::BSTreeConstIterator():
  node_(nullptr),
  fake_leaf_(nullptr)
{}

template< class Key, class Value >
typename ulanova::BSTreeConstIterator< Key, Value >::reference
ulanova::BSTreeConstIterator< Key, Value >::operator*() const
{
  return static_cast< detail::Node< Key, Value > * >(node_)->data;
}

template< class Key, class Value >
typename ulanova::BSTreeConstIterator< Key, Value >::pointer
ulanova::BSTreeConstIterator< Key, Value >::operator->() const
{
  return std::addressof(static_cast< detail::Node< Key, Value > * >(node_)->data);
}

template< class Key, class Value >
ulanova::BSTreeConstIterator< Key, Value > &
ulanova::BSTreeConstIterator< Key, Value >::operator++()
{
  if (node_->right != fake_leaf_)
  {
    node_ = node_->right;
    while (node_->left != fake_leaf_)
    {
      node_ = node_->left;
    }
  }
  else
  {
    detail::NodeBase * parent = node_->parent;
    while ((parent != nullptr) && (node_ == parent->right))
    {
      node_ = parent;
      parent = parent->parent;
    }

    if ((parent == nullptr) || (parent->parent == nullptr))
    {
      node_ = fake_leaf_;
    }
    else
    {
      node_ = parent;
    }
  }

  return *this;
}

template< class Key, class Value >
ulanova::BSTreeConstIterator< Key, Value >
ulanova::BSTreeConstIterator< Key, Value >::operator++(int)
{
  BSTreeConstIterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
bool ulanova::BSTreeConstIterator< Key, Value >::operator==(
  const BSTreeConstIterator & rhs
) const noexcept
{
  return node_ == rhs.node_;
}

template< class Key, class Value >
bool ulanova::BSTreeConstIterator< Key, Value >::operator!=(
  const BSTreeConstIterator & rhs
) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value >
ulanova::BSTreeConstIterator< Key, Value >::BSTreeConstIterator(
  detail::NodeBase * node,
  detail::NodeBase * fake_leaf
):
  node_(node),
  fake_leaf_(fake_leaf)
{}

template< class Key, class Value, class Compare >
typename ulanova::BSTree< Key, Value, Compare >::const_iterator
ulanova::BSTree< Key, Value, Compare >::cbegin() const noexcept
{
  return const_iterator(min_node(fake_root_->left), fake_leaf_);
}

template< class Key, class Value, class Compare >
typename ulanova::BSTree< Key, Value, Compare >::const_iterator
ulanova::BSTree< Key, Value, Compare >::cend() const noexcept
{
  return const_iterator(fake_leaf_, fake_leaf_);
}

template< class Key, class Value, class Compare >
typename ulanova::BSTree< Key, Value, Compare >::const_iterator
ulanova::BSTree< Key, Value, Compare >::begin() const noexcept
{
  return cbegin();
}

template< class Key, class Value, class Compare >
typename ulanova::BSTree< Key, Value, Compare >::const_iterator
ulanova::BSTree< Key, Value, Compare >::end() const noexcept
{
  return cend();
}

template< class Key, class Value, class Compare >
ulanova::detail::NodeBase *
ulanova::BSTree< Key, Value, Compare >::min_node(detail::NodeBase * node) const noexcept
{
  if (node == fake_leaf_)
  {
    return fake_leaf_;
  }

  while (node->left != fake_leaf_)
  {
    node = node->left;
  }

  return node;
}

template< class Key, class Value, class Compare >
ulanova::detail::NodeBase *
ulanova::BSTree< Key, Value, Compare >::next_node(detail::NodeBase * node) const noexcept
{
  if (node->right != fake_leaf_)
  {
    return min_node(node->right);
  }

  detail::NodeBase * parent = node->parent;
  while ((parent != nullptr) && (node == parent->right))
  {
    node = parent;
    parent = parent->parent;
  }

  if ((parent == nullptr) || (parent->parent == nullptr))
  {
    return fake_leaf_;
  }

  return parent;
}

template< class Key, class Value >
ulanova::BSTreeIterator< Key, Value >::BSTreeIterator():
  node_(nullptr),
  fake_leaf_(nullptr)
{}

template< class Key, class Value >
typename ulanova::BSTreeIterator< Key, Value >::reference
ulanova::BSTreeIterator< Key, Value >::operator*() const
{
  return static_cast< detail::Node< Key, Value > * >(node_)->data;
}

template< class Key, class Value >
typename ulanova::BSTreeIterator< Key, Value >::pointer
ulanova::BSTreeIterator< Key, Value >::operator->() const
{
  return &(static_cast< detail::Node< Key, Value > * >(node_)->data);
}

template< class Key, class Value >
ulanova::BSTreeIterator< Key, Value > &
ulanova::BSTreeIterator< Key, Value >::operator++()
{
  if (node_->right != fake_leaf_)
  {
    node_ = node_->right;
    while (node_->left != fake_leaf_)
    {
      node_ = node_->left;
    }
  }
  else
  {
    detail::NodeBase * parent = node_->parent;
    while ((parent != nullptr) && (node_ == parent->right))
    {
      node_ = parent;
      parent = parent->parent;
    }

    if ((parent == nullptr) || (parent->parent == nullptr))
    {
      node_ = fake_leaf_;
    }
    else
    {
      node_ = parent;
    }
  }

  return *this;
}

template< class Key, class Value >
ulanova::BSTreeIterator< Key, Value >
ulanova::BSTreeIterator< Key, Value >::operator++(int)
{
  BSTreeIterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
bool ulanova::BSTreeIterator< Key, Value >::operator==(
  const BSTreeIterator & rhs
) const noexcept
{
  return node_ == rhs.node_;
}

template< class Key, class Value >
bool ulanova::BSTreeIterator< Key, Value >::operator!=(
  const BSTreeIterator & rhs
) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value >
ulanova::BSTreeIterator< Key, Value >::BSTreeIterator(
  detail::NodeBase * node,
  detail::NodeBase * fake_leaf
):
  node_(node),
  fake_leaf_(fake_leaf)
{}

template< class Key, class Value, class Compare >
typename ulanova::BSTree< Key, Value, Compare >::iterator
ulanova::BSTree< Key, Value, Compare >::begin() noexcept
{
  return iterator(min_node(fake_root_->left), fake_leaf_);
}

template< class Key, class Value, class Compare >
typename ulanova::BSTree< Key, Value, Compare >::iterator
ulanova::BSTree< Key, Value, Compare >::end() noexcept
{
  return iterator(fake_leaf_, fake_leaf_);
}

template< class Key, class Value, class Compare >
Value ulanova::BSTree< Key, Value, Compare >::drop(Key key)
{
  detail::NodeBase * node = find_node(key);
  if (node == fake_leaf_)
  {
    throw std::out_of_range("key not found");
  }

  Value result = as_node(node)->data.second;
  detail::NodeBase * height_start = node->parent;

  if (node->left == fake_leaf_)
  {
    replace_node(node, node->right);
    delete node;
  }
  else if (node->right == fake_leaf_)
  {
    replace_node(node, node->left);
    delete node;
  }
  else
  {
    detail::NodeBase * next = min_node(node->right);
    height_start = next->parent;

    if (next->parent != node)
    {
      replace_node(next, next->right);
      next->right = node->right;
      next->right->parent = next;
    }
    else
    {
      height_start = next;
    }

    replace_node(node, next);
    next->left = node->left;
    next->left->parent = next;
    next->height = node->height;

    delete node;
  }

  --size_;

  while (height_start != nullptr)
  {
    update_height(height_start);
    height_start = height_start->parent;
  }

  return result;
}

template< class Key, class Value, class Compare >
void ulanova::BSTree< Key, Value, Compare >::replace_node(
  detail::NodeBase * old_node,
  detail::NodeBase * new_node
) noexcept
{
  if (old_node->parent == fake_root_)
  {
    fake_root_->left = new_node;
  }
  else if (old_node->parent->left == old_node)
  {
    old_node->parent->left = new_node;
  }
  else
  {
    old_node->parent->right = new_node;
  }

  if (new_node != fake_leaf_)
  {
    new_node->parent = old_node->parent;
  }
}

template< class Key, class Value, class Compare >
Value ulanova::BSTree< Key, Value, Compare >::get(Key key) const
{
  detail::NodeBase * node = find_node(key);
  if (node == fake_leaf_)
  {
    throw std::out_of_range("key not found");
  }

  return as_node(node)->data.second;
}

template< class Key, class Value, class Compare >
size_t ulanova::BSTree< Key, Value, Compare >::height() const noexcept
{
  return get_height(fake_root_->left);
}

template< class Key, class Value, class Compare >
size_t ulanova::BSTree< Key, Value, Compare >::height(const_iterator it) const noexcept
{
  return get_height(it.node_);
}

template< class Key, class Value, class Compare >
typename ulanova::BSTree< Key, Value, Compare >::const_iterator
ulanova::BSTree< Key, Value, Compare >::rotateLeft(const_iterator it)
{
  detail::NodeBase * node = it.node_;

  if ((node == fake_leaf_) || (node->right == fake_leaf_))
  {
    throw std::logic_error("left rotation is impossible");
  }

  detail::NodeBase * new_root = node->right;
  rotate_left_node(node);

  return const_iterator(new_root, fake_leaf_);
}

template< class Key, class Value, class Compare >
typename ulanova::BSTree< Key, Value, Compare >::const_iterator
ulanova::BSTree< Key, Value, Compare >::rotateRight(const_iterator it)
{
  detail::NodeBase * node = it.node_;

  if ((node == fake_leaf_) || (node->left == fake_leaf_))
  {
    throw std::logic_error("right rotation is impossible");
  }

  detail::NodeBase * new_root = node->left;
  rotate_right_node(node);

  return const_iterator(new_root, fake_leaf_);
}

template< class Key, class Value, class Compare >
typename ulanova::BSTree< Key, Value, Compare >::const_iterator
ulanova::BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator it)
{
  detail::NodeBase * node = it.node_;

  if ((node == fake_leaf_) || (node->right == fake_leaf_) || (node->right->left == fake_leaf_))
  {
    throw std::logic_error("large left rotation is impossible");
  }

  rotate_right_node(node->right);
  detail::NodeBase * new_root = node->right;
  rotate_left_node(node);

  return const_iterator(new_root, fake_leaf_);
}

template< class Key, class Value, class Compare >
typename ulanova::BSTree< Key, Value, Compare >::const_iterator
ulanova::BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator it)
{
  detail::NodeBase * node = it.node_;

  if ((node == fake_leaf_) || (node->left == fake_leaf_) || (node->left->right == fake_leaf_))
  {
    throw std::logic_error("large right rotation is impossible");
  }

  rotate_left_node(node->left);
  detail::NodeBase * new_root = node->left;
  rotate_right_node(node);

  return const_iterator(new_root, fake_leaf_);
}

template< class Key, class Value, class Compare >
void ulanova::BSTree< Key, Value, Compare >::rotate_left_node(detail::NodeBase * node)
{
  detail::NodeBase * new_root = node->right;

  node->right = new_root->left;
  if (new_root->left != fake_leaf_)
  {
    new_root->left->parent = node;
  }

  new_root->parent = node->parent;

  if (node->parent == fake_root_)
  {
    fake_root_->left = new_root;
  }
  else if (node->parent->left == node)
  {
    node->parent->left = new_root;
  }
  else
  {
    node->parent->right = new_root;
  }

  new_root->left = node;
  node->parent = new_root;

  update_height(node);
  update_height(new_root);
}

template< class Key, class Value, class Compare >
void ulanova::BSTree< Key, Value, Compare >::rotate_right_node(detail::NodeBase * node)
{
  detail::NodeBase * new_root = node->left;

  node->left = new_root->right;
  if (new_root->right != fake_leaf_)
  {
    new_root->right->parent = node;
  }

  new_root->parent = node->parent;

  if (node->parent == fake_root_)
  {
    fake_root_->left = new_root;
  }
  else if (node->parent->left == node)
  {
    node->parent->left = new_root;
  }
  else
  {
    node->parent->right = new_root;
  }

  new_root->right = node;
  node->parent = new_root;

  update_height(node);
  update_height(new_root);
}

template< class Key, class Value, class Compare >
ulanova::BSTree< Key, Value, Compare >::BSTree(const BSTree & other):
  fake_root_(nullptr),
  fake_leaf_(nullptr),
  size_(0),
  compare_(other.compare_)
{
  create_fakes();

  try
  {
    copy_from(other.fake_root_->left, other.fake_leaf_);
  }
  catch (...)
  {
    clear();
    delete fake_root_;
    delete fake_leaf_;
    fake_root_ = nullptr;
    fake_leaf_ = nullptr;
    throw;
  }
}

template< class Key, class Value, class Compare >
ulanova::BSTree< Key, Value, Compare > &
ulanova::BSTree< Key, Value, Compare >::operator=(const BSTree & other)
{
  if (this != &other)
  {
    BSTree temp(other);
    swap(temp);
  }

  return *this;
}

template< class Key, class Value, class Compare >
void ulanova::BSTree< Key, Value, Compare >::swap(BSTree & other) noexcept
{
  detail::NodeBase * temp_root = fake_root_;
  fake_root_ = other.fake_root_;
  other.fake_root_ = temp_root;

  detail::NodeBase * temp_leaf = fake_leaf_;
  fake_leaf_ = other.fake_leaf_;
  other.fake_leaf_ = temp_leaf;

  size_t temp_size = size_;
  size_ = other.size_;
  other.size_ = temp_size;

  Compare temp_compare = compare_;
  compare_ = other.compare_;
  other.compare_ = temp_compare;
}

template< class Key, class Value, class Compare >
void ulanova::BSTree< Key, Value, Compare >::copy_from(
  detail::NodeBase * node,
  detail::NodeBase * other_fake_leaf
)
{
  if (node == other_fake_leaf)
  {
    return;
  }

  node_t * typed_node = static_cast< node_t * >(node);

  push(typed_node->data.first, typed_node->data.second);
  copy_from(node->left, other_fake_leaf);
  copy_from(node->right, other_fake_leaf);
}

#endif
