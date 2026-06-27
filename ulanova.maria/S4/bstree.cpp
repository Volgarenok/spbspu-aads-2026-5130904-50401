#include "bstree.hpp"

ulanova::detail::NodeBase::NodeBase():
  parent(nullptr),
  left(nullptr),
  right(nullptr)
{}

ulanova::detail::NodeBase::NodeBase(
  NodeBase* parent_node,
  NodeBase* left_node,
  NodeBase* right_node
):
  parent(parent_node),
  left(left_node),
  right(right_node)
{}

ulanova::detail::NodeBase::~NodeBase() = default;
