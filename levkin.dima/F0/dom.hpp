#include "vector.hpp"
#include <string>
#ifndef DOM
#define DOM

namespace rl {

  struct RLNode {
    std::string id;
    stuff::Vector< RLNode* > children;
    void addChild(std::string id);
    void removeChild(std::string id);
    // TODO стили и размеры
  };

  struct RTRoot {
    RLNode root;
    // TODO: absolute
    // std::vector<El*>
  };

  enum class FlexDirection { Row, Column, RowReverse, ColumnReverse };

  enum class JustifyContent {
    FlexStart,
    FlexEnd,
    Center,
    SpaceBetween,
    SpaceAround
  };

  enum class AlignItems { FlexStart, FlexEnd, Center, Stretch };
  namespace rl {

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
  }
}

#endif
