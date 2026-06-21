#include "vector.hpp"
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
    stuff::Vector< RLNode* > children;
    RLNode* parent;

    void addChild(std::string id);
    void removeChild(std::string id);

    Sides margin;
    Sides padding;

    float width = -1.0;
    float height = -1.0;

    FlexDirection flexDirection = FlexDirection::Row;
    JustifyContent justify = JustifyContent::FlexStart;
    AlignItems align = AlignItems::FlexStart;

    ~RLNode();
  };
}
#endif
