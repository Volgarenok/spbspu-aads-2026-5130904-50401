#include "node.hpp"

namespace rl {
  void RLNode::addChild(std::unique_ptr< RLNode > p)
  {
    if (!p)
      return;

    p->parent = this;
    children.pushBack(std::move(p));
  }

  void RLNode::clearChildren() noexcept
  {
    children.erase(children.begin(), children.end());
  }

  void RLNode::removeChild(const std::string& child_id)
  {
    for (auto it = children.begin(); it != children.end(); ++it) {
      if ((*it)->id == child_id) {
        children.erase(it);
        return;
      }
    }
  }

  RLNode::RLNode(std::string nodeId, float w, float h, FlexDirection dir)
      : id(std::move(nodeId)), width(w), height(h), flexDirection(dir)
  {
  }

  RLNode::RLNode(RLNode&& rhs) noexcept
      : id(std::move(rhs.id)), children(std::move(rhs.children)),
        parent(rhs.parent), margin(rhs.margin), padding(rhs.padding),
        width(rhs.width), height(rhs.height), flexDirection(rhs.flexDirection),
        justify(rhs.justify), align(rhs.align), box(rhs.box)
  {
    updateChildrenParent();

    rhs.reset();
  }

  RLNode& RLNode::operator=(RLNode&& rhs) noexcept
  {
    if (this == &rhs) {
      return *this;
    }

    clearChildren();

    id = std::move(rhs.id);
    children = std::move(rhs.children);
    parent = rhs.parent;
    margin = rhs.margin;
    padding = rhs.padding;
    width = rhs.width;
    height = rhs.height;
    flexDirection = rhs.flexDirection;
    justify = rhs.justify;
    align = rhs.align;
    box = rhs.box;

    updateChildrenParent();
    rhs.reset();

    return *this;
  }
  void RLNode::updateChildrenParent() noexcept
  {
    for (size_t i = 0; i < children.getSize(); ++i) {
      if (children[i]) {
        children[i]->parent = this;
      }
    }
  }
  void RLNode::reset() noexcept
  {
    parent = nullptr;
    width = -1.0f;
    height = -1.0f;
    box.x = 0.0f;
    box.y = 0.0f;
    box.width = 0.0f;
    box.height = 0.0f;
  }
}
