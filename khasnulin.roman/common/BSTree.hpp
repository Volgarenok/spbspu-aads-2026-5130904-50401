#ifndef BSTREE_HPP
#define BSTREE_HPP

#include "BSNode.hpp"
#include "BSTreeIter.hpp"
#include <Queue.hpp>
#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace khasnulin
{
  template < class Key, class Value > using BSTreeIter = BSTreeIterator< Key, Value, false >;

  template < class Key, class Value > using CBSTreeIter = BSTreeIterator< Key, Value, true >;

  template < class Key, class Value, class Compare = std::less< Key > > class BSTree
  {

    static_assert(std::is_nothrow_destructible< Key >::value && std::is_nothrow_destructible< Value >::value,
                  "BSTree: Key and Value destructors must not throw");
    static_assert(std::is_nothrow_move_constructible< Compare >::value &&
                      std::is_nothrow_move_assignable< Compare >::value,
                  "BSTree: Compare must be nothrow movable (required by noexcept move/swap)");

  public:
    using key_type = Key;
    using mapped_type = Value;
    using size_type = std::size_t;

    using value_type = std::pair< const Key, Value >;

    using comparator = Compare;

    using iter = BSTreeIter< Key, Value >;
    using const_iter = CBSTreeIter< Key, Value >;

    BSTree();

    explicit BSTree(Compare comparator);

    BSTree(const BSTree &rhs);
    BSTree(BSTree &&rhs) noexcept;

    BSTree &operator=(const BSTree &rhs);
    // после move перемещённый объект пуст.
    BSTree &operator=(BSTree &&rhs) noexcept;

    ~BSTree() noexcept;

    bool empty() const noexcept;
    size_type size() const noexcept;

    // Возвращает высоту самого длинного пути от корня в дереве
    size_type height() const;
    // высота поддерева с корнем в it; 0 для end(); пустое дерево — 0, один узел — 1
    size_type height(const_iter it) const;

    // итераторы начала обхода дерева
    iter begin() noexcept;
    const_iter begin() const noexcept;
    const_iter cbegin() const noexcept;

    iter end() noexcept;
    const_iter end() const noexcept;
    const_iter cend() const noexcept;

    // получить значение по ключу
    mapped_type &get(const Key &k);
    const mapped_type &get(const Key &k) const;

    // получить значение по ключу, если такого нет -- создать такой узел с пустым значением
    mapped_type &operator[](const Key &k);

    // найти элемент в дереве по ключу, вернуть итератор, если элемента нет, итератор в end
    iter find(const Key &k);
    const_iter find(const Key &k) const;

    // вставить элемент value в дереве по ключу и вернуть итератор на него. Если key уже есть в дереве, просто
    // возвращает iter на этот элемент без изменений.
    iter insert(const value_type &pair);
    iter insert(value_type &&pair);

    // удалить элемент с ключом k из дерева
    size_type erase(const Key &k);
    // удалить элемент по итератору. вернуть итератор на следующий элемент
    iter erase(const_iter it);

    // очистить полностью дерево от всех узлов
    void clear() noexcept;

    // swap для ADL
    void swap(BSTree &rhs) noexcept;

    // 4 метода поворотов для будущей обретки других деревьев. аргумент — опускающийся узел; возвращается
    // новый корень поддерева; если нужного ребёнка нет, бросается std::logic_error, дерево не меняется
    iter rotateLeft(const_iter it);
    iter rotateRight(const_iter it);
    iter rotateLargeLeft(const_iter it);
    iter rotateLargeRight(const_iter it);

    // методы обхода разными способами с применением функционального объекта f
    // может бросить при выделении памяти или из f; дерево не меняется
    template < class F > F traverseLNR(F f) const;
    template < class F > F traverseRNL(F f) const;
    template < class F > F traverseBreadth(F f) const;

  private:
    struct FindResult
    {
      Node< Key, Value > *node;
      NodeBase< Key, Value > *parent;
      bool isLeft;
    };
    // const метод возвращает не const данные. Сделано специально для внутренного использования.
    FindResult findByKey(const Key &k) const;
    template < class P > iter insertImpl(P &&pair);
    void addNewNodeToTree(Node< Key, Value > *node, NodeBase< Key, Value > *parent, bool isLeft) noexcept;

    void unlinkAndDeleteNode(Node< Key, Value > *node) noexcept;

    size_type size_;

    NodeBase< Key, Value > fakeRoot_;

    Compare comparator_;
  };

  template < class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::unlinkAndDeleteNode(Node< Key, Value > *node) noexcept
  {
    assert(node != nullptr && node != &fakeRoot_);
    NodeBase< Key, Value > *parent = node->parent;
    Node< Key, Value > *oldNode = node;
    Node< Key, Value > *newNode = oldNode->left ? oldNode->left : oldNode->right;
    bool isLeft = (parent->left == node);
    if (oldNode->left && oldNode->right)
    {
      newNode = minNode(oldNode->right);
      oldNode->left->parent = newNode;
      newNode->left = oldNode->left;
      if (newNode != oldNode->right)
      {
        newNode->parent->left = newNode->right;
        if (newNode->right)
        {
          newNode->right->parent = newNode->parent;
        }
        newNode->right = oldNode->right;
        oldNode->right->parent = newNode;
      }
    }
    if (isLeft)
    {
      parent->left = newNode;
    }
    else
    {
      parent->right = newNode;
    }
    if (newNode)
    {
      newNode->parent = parent;
    }
    delete oldNode;
    --size_;
  }

  template < class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
      size_(0),
      fakeRoot_(),
      comparator_()
  {
  }

  template < class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(Compare comparator):
      size_(0),
      fakeRoot_(),
      comparator_(comparator)
  {
  }

  template < class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree &rhs):
      size_(0),
      fakeRoot_(),
      comparator_(rhs.comparator_)
  {
    try
    {
      const NodeBase< Key, Value > *currRhs = rhs.fakeRoot_.left;
      const NodeBase< Key, Value > *prevRhs = &rhs.fakeRoot_;
      NodeBase< Key, Value > *curr = &fakeRoot_;
      while (currRhs && currRhs != &rhs.fakeRoot_)
      {
        if (prevRhs == currRhs->parent)
        {
          Node< Key, Value > *newNode =
              new Node< Key, Value >(static_cast< const Node< Key, Value > * >(currRhs)->val);
          if (currRhs == prevRhs->left)
          {
            curr->left = newNode;
            newNode->parent = curr;
          }
          else
          {
            curr->right = newNode;
            newNode->parent = curr;
          }
          curr = newNode;
          ++size_;
          prevRhs = currRhs;
          if (currRhs->left)
          {
            currRhs = currRhs->left;
          }
          else if (currRhs->right)
          {
            currRhs = currRhs->right;
          }
          else
          {
            currRhs = currRhs->parent;
            curr = curr->parent;
          }
        }
        else
        {
          if (currRhs->left == prevRhs && currRhs->right)
          {
            prevRhs = currRhs;
            currRhs = currRhs->right;
          }
          else
          {
            prevRhs = currRhs;
            currRhs = currRhs->parent;
            curr = curr->parent;
          }
        }
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template < class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(BSTree &&rhs) noexcept:
      size_(rhs.size_),
      fakeRoot_(),
      comparator_(std::move(rhs.comparator_))
  {
    fakeRoot_.left = rhs.fakeRoot_.left;
    if (fakeRoot_.left != nullptr)
    {
      fakeRoot_.left->parent = &fakeRoot_;
    }
    rhs.size_ = 0;
    rhs.fakeRoot_.left = nullptr;
  }

  template < class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > &BSTree< Key, Value, Compare >::operator=(const BSTree &rhs)
  {
    if (std::addressof(rhs) == this)
    {
      return *this;
    }
    BSTree tmp(rhs);
    swap(tmp);
    return *this;
  }

  template < class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > &BSTree< Key, Value, Compare >::operator=(BSTree &&rhs) noexcept
  {
    if (std::addressof(rhs) == this)
    {
      return *this;
    }
    clear();
    swap(rhs);
    return *this;
  }

  template < class Key, class Value, class Compare > BSTree< Key, Value, Compare >::~BSTree() noexcept
  {
    clear();
  }

  template < class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::size_type BSTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::size_type BSTree< Key, Value, Compare >::height() const
  {
    if (empty())
    {
      return 0;
    }
    return height(const_iter(fakeRoot_.left));
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::size_type BSTree< Key, Value, Compare >::height(const_iter it) const
  {
    if (!it.node_)
    {
      throw std::logic_error("can't get height of empty iter");
    }
    if (it == end())
    {
      return 0;
    }
    size_type maxHeight = 0;
    const Node< Key, Value > *curr = static_cast< const Node< Key, Value > * >(it.node_);
    size_type currHeight = 1;
    while (true)
    {
      if (curr->left)
      {
        curr = curr->left;
        ++currHeight;
      }
      else if (curr->right)
      {
        curr = curr->right;
        ++currHeight;
      }
      else
      {
        maxHeight = maxHeight < currHeight ? currHeight : maxHeight;
        bool isFoundNextRight = false;
        while (!isFoundNextRight)
        {
          if (curr == it.node_)
          {
            return maxHeight;
          }
          const NodeBase< Key, Value > *parent = curr->parent;
          bool isLeft = (parent->left == curr);
          curr = static_cast< const Node< Key, Value > * >(parent);
          --currHeight;
          if (isLeft && curr->right)
          {
            isFoundNextRight = true;
            curr = curr->right;
            ++currHeight;
          }
        }
      }
    }
    return maxHeight;
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iter BSTree< Key, Value, Compare >::begin() noexcept
  {
    NodeBase< Key, Value > *min = minNode(&fakeRoot_);
    return iter(min);
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iter BSTree< Key, Value, Compare >::begin() const noexcept
  {
    return const_iter(const_cast< BSTree< Key, Value, Compare > * >(this)->begin());
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iter BSTree< Key, Value, Compare >::cbegin() const noexcept
  {
    return const_iter(begin());
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iter BSTree< Key, Value, Compare >::end() noexcept
  {
    return iter(&fakeRoot_);
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iter BSTree< Key, Value, Compare >::end() const noexcept
  {
    return const_iter(&fakeRoot_);
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iter BSTree< Key, Value, Compare >::cend() const noexcept
  {
    return const_iter(end());
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::mapped_type &BSTree< Key, Value, Compare >::get(const Key &k)
  {
    FindResult result = findByKey(k);
    if (!result.node)
    {
      throw std::out_of_range("can't find node with such key in tree");
    }
    return result.node->val.second;
  }

  template < class Key, class Value, class Compare >
  const typename BSTree< Key, Value, Compare >::mapped_type &
  BSTree< Key, Value, Compare >::get(const Key &k) const
  {
    FindResult result = findByKey(k);
    if (!result.node)
    {
      throw std::out_of_range("can't find node with such key in tree");
    }
    return result.node->val.second;
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::mapped_type &BSTree< Key, Value, Compare >::operator[](const Key &k)
  {
    FindResult result = findByKey(k);
    if (result.node)
    {
      return result.node->val.second;
    }
    Node< Key, Value > *newNode = new Node< Key, Value >({k, Value()});
    addNewNodeToTree(newNode, result.parent, result.isLeft);
    return newNode->val.second;
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::FindResult
  BSTree< Key, Value, Compare >::findByKey(const Key &k) const
  {
    NodeBase< Key, Value > *parent = const_cast< NodeBase< Key, Value > * >(&fakeRoot_);
    Node< Key, Value > *curr = fakeRoot_.left;
    bool isLeft = true;
    while (curr)
    {
      if (comparator_(k, curr->val.first))
      {
        parent = curr;
        curr = curr->left;
        isLeft = true;
      }
      else if (comparator_(curr->val.first, k))
      {
        parent = curr;
        curr = curr->right;
        isLeft = false;
      }
      else
      {
        return {curr, parent, isLeft};
      }
    }
    return {nullptr, parent, isLeft};
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iter BSTree< Key, Value, Compare >::find(const Key &k)
  {
    FindResult result = findByKey(k);
    return result.node ? iter(result.node) : end();
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iter BSTree< Key, Value, Compare >::find(const Key &k) const
  {
    FindResult result = findByKey(k);
    return result.node ? const_iter(result.node) : end();
  }

  template < class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::addNewNodeToTree(Node< Key, Value > *node,
                                                       NodeBase< Key, Value > *parent, bool isLeft) noexcept
  {
    if (isLeft)
    {
      parent->left = node;
    }
    else
    {
      parent->right = node;
    }
    node->parent = parent;
    ++size_;
  }

  template < class Key, class Value, class Compare >
  template < class P >
  typename BSTree< Key, Value, Compare >::iter BSTree< Key, Value, Compare >::insertImpl(P &&pair)
  {
    FindResult result = findByKey(pair.first);
    if (result.node)
    {
      return iter(result.node);
    }
    Node< Key, Value > *newNode = new Node< Key, Value >(std::forward< P >(pair));
    addNewNodeToTree(newNode, result.parent, result.isLeft);
    return iter(newNode);
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iter BSTree< Key, Value, Compare >::insert(const value_type &pair)
  {
    return insertImpl(pair);
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iter BSTree< Key, Value, Compare >::insert(value_type &&pair)
  {
    return insertImpl(std::move(pair));
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::size_type BSTree< Key, Value, Compare >::erase(const Key &k)
  {
    FindResult result = findByKey(k);
    if (!result.node)
    {
      return 0;
    }
    unlinkAndDeleteNode(result.node);
    return 1;
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iter BSTree< Key, Value, Compare >::erase(const_iter it)
  {
    if (!it.node_ || it == end())
    {
      return end();
    }
    NodeBase< Key, Value > *base = const_cast< NodeBase< Key, Value > * >(it.node_);
    NodeBase< Key, Value > *next = nextNode(base);
    unlinkAndDeleteNode(static_cast< Node< Key, Value > * >(base));
    return iter(next);
  }

  template < class Key, class Value, class Compare > void BSTree< Key, Value, Compare >::clear() noexcept
  {
    Node< Key, Value > *curr = fakeRoot_.left;
    while (fakeRoot_.left != nullptr)
    {
      if (curr->left)
      {
        curr = curr->left;
      }
      else if (curr->right)
      {
        curr = curr->right;
      }
      else
      {
        NodeBase< Key, Value > *parent = curr->parent;
        unlinkAndDeleteNode(curr);
        curr = (parent == &fakeRoot_) ? nullptr : static_cast< Node< Key, Value > * >(parent);
      }
    }
  }

  template < class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree &rhs) noexcept
  {
    if (std::addressof(rhs) == this)
    {
      return;
    }

    std::swap(fakeRoot_.left, rhs.fakeRoot_.left);
    if (fakeRoot_.left)
    {
      fakeRoot_.left->parent = &fakeRoot_;
    }
    if (rhs.fakeRoot_.left)
    {
      rhs.fakeRoot_.left->parent = &rhs.fakeRoot_;
    }

    std::swap(size_, rhs.size_);
    std::swap(comparator_, rhs.comparator_);
  }

  // 4 метода поворотов для будущей обретки других деревьев.
  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iter BSTree< Key, Value, Compare >::rotateLeft(const_iter it)
  {
    if (!it.node_ || !it.node_->parent || !it.node_->right)
    {
      throw std::logic_error("rotate left: can't make rotate, some nodes doesn't exists");
    }
    Node< Key, Value > *oldRoot =
        static_cast< Node< Key, Value > * >(const_cast< NodeBase< Key, Value > * >(it.node_));
    NodeBase< Key, Value > *parent = oldRoot->parent;
    Node< Key, Value > *newRoot = oldRoot->right;
    replaceChild(parent, oldRoot, newRoot);
    setRight(oldRoot, newRoot->left);
    setLeft(newRoot, oldRoot);
    return iter(newRoot);
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iter BSTree< Key, Value, Compare >::rotateRight(const_iter it)
  {
    if (!it.node_ || !it.node_->parent || !it.node_->left)
    {
      throw std::logic_error("rotate right: can't make rotate, some nodes doesn't exists");
    }
    Node< Key, Value > *oldRoot =
        static_cast< Node< Key, Value > * >(const_cast< NodeBase< Key, Value > * >(it.node_));
    NodeBase< Key, Value > *parent = oldRoot->parent;
    Node< Key, Value > *newRoot = oldRoot->left;
    replaceChild(parent, oldRoot, newRoot);
    setLeft(oldRoot, newRoot->right);
    setRight(newRoot, oldRoot);
    return iter(newRoot);
  }

  // Что сделано: большие повороты теперь выражены через малые
  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iter BSTree< Key, Value, Compare >::rotateLargeLeft(const_iter it)
  {
    if (!it.node_)
    {
      throw std::logic_error("large left rotate: invalid iterator");
    }
    rotateRight(const_iter(it.node_->right));
    return rotateLeft(it);
  }

  template < class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iter BSTree< Key, Value, Compare >::rotateLargeRight(const_iter it)
  {
    if (!it.node_)
    {
      throw std::logic_error("large right rotate: invalid iterator");
    }
    rotateLeft(const_iter(it.node_->left));
    return rotateRight(it);
  }

  template < class Key, class Value, class Compare >
  template < class F >
  F BSTree< Key, Value, Compare >::traverseLNR(F f) const
  {
    for (const_iter it = begin(), e = end(); it != e; ++it)
    {
      f(*it);
    }
    return f;
  }

  template < class Key, class Value, class Compare >
  template < class F >
  F BSTree< Key, Value, Compare >::traverseRNL(F f) const
  {
    const_iter it = end();
    const_iter beg = begin();
    while (it != beg)
    {
      --it;
      f(*it);
    }
    return f;
  }

  template < class Key, class Value, class Compare >
  template < class F >
  F BSTree< Key, Value, Compare >::traverseBreadth(F f) const
  {
    if (empty())
    {
      return f;
    }
    Queue< const Node< Key, Value > * > queue;
    queue.push(fakeRoot_.left);
    while (!queue.empty())
    {
      const Node< Key, Value > *node = queue.front();
      queue.pop();
      f(node->val);
      if (node->left)
      {
        queue.push(node->left);
      }
      if (node->right)
      {
        queue.push(node->right);
      }
    }
    return f;
  }

  template < class Key, class Value, class Compare >
  void swap(BSTree< Key, Value, Compare > &lhs, BSTree< Key, Value, Compare > &rhs) noexcept
  {
    lhs.swap(rhs);
  }
}

#endif
