#include "node.hpp"

namespace rl {
  void RLNode::addChild(std::unique_ptr< RLNode > p)
  {
    if (!p)
      return;

    p->parent = this;
    children.pushBack(std::move(p));
  }
  void RLNode::clearChildren()
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
}
