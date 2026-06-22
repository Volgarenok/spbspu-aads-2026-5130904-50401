#ifndef NODE_PROPERTIES
#define NODE_PROPERTIES

namespace rl {
struct Size {
  enum Type { Pixels, Auto } type = Auto;
  float value = 0.0f;
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
#endif
