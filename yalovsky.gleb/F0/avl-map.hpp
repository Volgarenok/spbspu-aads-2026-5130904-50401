#ifndef YALOVSKY_F0_AVL_MAP_HPP
#define YALOVSKY_F0_AVL_MAP_HPP

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

namespace yalovsky
{
  template< class Key, class Value >
  class AvlMap
  {
  private:
    struct Node
    {
      const Key key;
      Value value;
      Node* left;
      Node* right;
      int height;

      Node(const Key& newKey, const Value& newValue):
        key(newKey),
        value(newValue),
        left(nullptr),
        right(nullptr),
        height(1)
      {}

      Node(const Key& newKey, Value&& newValue):
        key(newKey),
        value(std::move(newValue)),
        left(nullptr),
        right(nullptr),
        height(1)
      {}

      ~Node()
      {
        delete left;
        delete right;
      }

      Node(const Node&) = delete;
      Node& operator=(const Node&) = delete;
    };

    Node* root_ = nullptr;
    std::size_t size_ = 0;

    static int getNodeHeight(const Node* node) noexcept
    {
      return node ? node->height : 0;
    }

    static void updateHeight(Node& node) noexcept
    {
      node.height = 1 + std::max(
          getNodeHeight(node.left),
          getNodeHeight(node.right));
    }

    static void rotateLeft(Node*& subtree) noexcept
    {
      Node* newRoot = subtree->right;

      subtree->right = newRoot->left;
      newRoot->left = subtree;

      updateHeight(*subtree);
      updateHeight(*newRoot);
      subtree = newRoot;
    }

    static void rotateRight(Node*& subtree) noexcept
    {
      Node* newRoot = subtree->left;

      subtree->left = newRoot->right;
      newRoot->right = subtree;

      updateHeight(*subtree);
      updateHeight(*newRoot);
      subtree = newRoot;
    }

    static void balance(Node*& subtree) noexcept
    {
      if (!subtree)
      {
        return;
      }

      updateHeight(*subtree);

      const int difference =
          getNodeHeight(subtree->right) - getNodeHeight(subtree->left);

      if (difference > 1)
      {
        if (getNodeHeight(subtree->right->left) >
            getNodeHeight(subtree->right->right))
        {
          rotateRight(subtree->right);
        }

        rotateLeft(subtree);
      }
      else if (difference < -1)
      {
        if (getNodeHeight(subtree->left->right) >
            getNodeHeight(subtree->left->left))
        {
          rotateLeft(subtree->left);
        }

        rotateRight(subtree);
      }
    }

    static Node* clone(const Node* source)
    {
      if (!source)
      {
        return nullptr;
      }

      Node* copy = new Node(source->key, source->value);

      try
      {
        copy->left = clone(source->left);
        copy->right = clone(source->right);
        copy->height = source->height;
      }
      catch (...)
      {
        delete copy;
        throw;
      }

      return copy;
    }

    template< class NodeType >
    static NodeType* findNode(NodeType* current, const Key& key)
    {
      while (current)
      {
        if (key < current->key)
        {
          current = current->left;
        }
        else if (current->key < key)
        {
          current = current->right;
        }
        else
        {
          return current;
        }
      }

      return nullptr;
    }

    template< class ValueArgument >
    static bool insertNode(
        Node*& subtree,
        const Key& key,
        ValueArgument&& value)
    {
      if (!subtree)
      {
        subtree = new Node(
            key,
            std::forward< ValueArgument >(value));
        return true;
      }

      bool inserted = false;

      if (key < subtree->key)
      {
        inserted = insertNode(
            subtree->left,
            key,
            std::forward< ValueArgument >(value));
      }
      else if (subtree->key < key)
      {
        inserted = insertNode(
            subtree->right,
            key,
            std::forward< ValueArgument >(value));
      }
      else
      {
        return false;
      }

      balance(subtree);
      return inserted;
    }

    static Node* takeMin(Node*& subtree) noexcept
    {
      if (!subtree->left)
      {
        Node* minimum = subtree;
        subtree = minimum->right;
        minimum->right = nullptr;
        return minimum;
      }

      Node* minimum = takeMin(subtree->left);
      balance(subtree);
      return minimum;
    }

    static bool eraseNode(Node*& subtree, const Key& key)
    {
      if (!subtree)
      {
        return false;
      }

      bool removed = false;

      if (key < subtree->key)
      {
        removed = eraseNode(subtree->left, key);
      }
      else if (subtree->key < key)
      {
        removed = eraseNode(subtree->right, key);
      }
      else
      {
        Node* oldNode = subtree;

        if (!oldNode->left || !oldNode->right)
        {
          subtree = oldNode->left ?
              oldNode->left : oldNode->right;

          oldNode->left = nullptr;
          oldNode->right = nullptr;
          delete oldNode;
        }
        else
        {
          Node* replacement = takeMin(oldNode->right);
          replacement->left = oldNode->left;
          replacement->right = oldNode->right;

          oldNode->left = nullptr;
          oldNode->right = nullptr;
          subtree = replacement;
          delete oldNode;
        }

        removed = true;
      }

      balance(subtree);
      return removed;
    }

    template< class NodeType, class Function >
    static void traverse(NodeType* root, Function& function)
    {
      std::vector< NodeType* > path;
      NodeType* current = root;

      while (current || !path.empty())
      {
        while (current)
        {
          path.push_back(current);
          current = current->left;
        }

        current = path.back();
        path.pop_back();

        function(current->key, current->value);
        current = current->right;
      }
    }

  public:
    AvlMap() = default;

    ~AvlMap() noexcept
    {
      clear();
    }

    AvlMap(const AvlMap& other):
      root_(clone(other.root_)),
      size_(other.size_)
    {}

    AvlMap(AvlMap&& other) noexcept:
      root_(other.root_),
      size_(other.size_)
    {
      other.root_ = nullptr;
      other.size_ = 0;
    }

    AvlMap& operator=(const AvlMap& other)
    {
      if (this != &other)
      {
        AvlMap copy(other);
        swap(copy);
      }

      return *this;
    }

    AvlMap& operator=(AvlMap&& other) noexcept
    {
      if (this != &other)
      {
        clear();

        root_ = other.root_;
        size_ = other.size_;

        other.root_ = nullptr;
        other.size_ = 0;
      }

      return *this;
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    std::size_t size() const noexcept
    {
      return size_;
    }

    bool contains(const Key& key) const
    {
      const Node* root = root_;
      return findNode(root, key) != nullptr;
    }

    Value& at(const Key& key)
    {
      Node* node = findNode(root_, key);

      if (!node)
      {
        throw std::out_of_range("Key not found");
      }

      return node->value;
    }

    const Value& at(const Key& key) const
    {
      const Node* root = root_;
      const Node* node = findNode(root, key);

      if (!node)
      {
        throw std::out_of_range("Key not found");
      }

      return node->value;
    }

    bool insert(const Key& key, const Value& value)
    {
      const bool inserted = insertNode(root_, key, value);

      if (inserted)
      {
        ++size_;
      }

      return inserted;
    }

    bool insert(const Key& key, Value&& value)
    {
      const bool inserted = insertNode(
          root_,
          key,
          std::move(value));

      if (inserted)
      {
        ++size_;
      }

      return inserted;
    }

    bool erase(const Key& key)
    {
      const bool removed = eraseNode(root_, key);

      if (removed)
      {
        --size_;
      }

      return removed;
    }

    void clear() noexcept
    {
      delete root_;
      root_ = nullptr;
      size_ = 0;
    }

    void swap(AvlMap& other) noexcept
    {
      std::swap(root_, other.root_);
      std::swap(size_, other.size_);
    }

    template< class Function >
    void forEach(Function function)
    {
      traverse(root_, function);
    }

    template< class Function >
    void forEach(Function function) const
    {
      const Node* root = root_;
      traverse(root, function);
    }
  };
}

#endif