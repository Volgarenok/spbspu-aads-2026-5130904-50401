#ifndef NODE_RL
#define NODE_RL

#include "node-properties.hpp"
#include "vector.hpp"
#include <memory>
#include <string>

namespace rl {

  struct RLNode {
    std::string id;
    stf::Vector< std::unique_ptr< RLNode > > children;
    RLNode* parent = nullptr;

    void addChild(std::unique_ptr< RLNode > child);
    void removeChild(const std::string& nodeId);
    void clearChildren() noexcept;

    Sides margin;
    Sides padding;

    float width = -1.0f;
    float height = -1.0f;

    FlexDirection flexDirection = FlexDirection::Row;
    JustifyContent justify = JustifyContent::FlexStart;
    AlignItems align = AlignItems::FlexStart;

    Rect box;

    RLNode() = default;
    RLNode(std::string nodeId, float w, float h, FlexDirection dir);
    ~RLNode() = default;

    RLNode(const RLNode&) = delete;
    RLNode& operator=(const RLNode&) = delete;

    RLNode(RLNode&& rhs) noexcept;
    RLNode& operator=(RLNode&& rhs) noexcept;

  private:
    void updateChildrenParent() noexcept;
    void reset() noexcept;
  };

}

#endif
