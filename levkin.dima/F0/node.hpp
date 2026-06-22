#include "vector.hpp"
#include <memory>
#include <string>
#ifndef DOM
#define DOM

namespace rl {

  enum class FlexDirection { Row, Column, RowReverse, ColumnReverse };

  enum class JustifyContent {
    FlexStart,
    FlexEnd,
    Center,
    SpaceBetween,
    SpaceAround
  };

  enum class AlignItems { FlexStart, FlexEnd, Center, Stretch };

  struct Sides {
    float left = 0.0f;
    float right = 0.0f;
    float top = 0.0f;
    float bottom = 0.0f;
  };

  struct Rect {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
  };

  struct RLNode {
    std::string id;
    stf::Vector< std::unique_ptr< RLNode > > children;
    RLNode* parent = nullptr;

    void addChild(std::unique_ptr< RLNode >);
    void removeChild(const std::string& id);
    void clearChildren();

    Sides margin;
    Sides padding;
    Rect box;
    float width = -1.0;
    float height = -1.0;

    FlexDirection flexDirection = FlexDirection::Row;
    JustifyContent justify = JustifyContent::FlexStart;
    AlignItems align = AlignItems::FlexStart;

    RLNode() = default;
    RLNode(std::string nodeId, float w, float h, FlexDirection dir);
    ~RLNode() = default;
    RLNode(const RLNode&) = delete;
    RLNode& operator=(const stf::Vector< RLNode >&) = delete;
    RLNode(RLNode&& rhs) noexcept;
    RLNode& operator=(RLNode&& rhs) noexcept;

  private:
    void updateChildrenParent() noexcept;
    void reset() noexcept;
  };
}
#endif
