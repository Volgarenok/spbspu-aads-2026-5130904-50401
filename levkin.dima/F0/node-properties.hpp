#ifndef NODE_PROPERTIES
#define NODE_PROPERTIES

#include <iosfwd>
namespace rl {
  enum class SizeType { Pixels, Auto };
  struct Size {
    SizeType type = SizeType::Auto;
    float value = 0.0f;

    Size& operator=(float val)
    {
      if (val < 0.0f) {
        type = SizeType::Auto;
        value = 0.0f;
      } else {
        type = SizeType::Pixels;
        value = val;
      }
      return *this;
    }
    static Size fromFloat(float val)
    {
      if (val < 0.0f)
        return {SizeType::Auto, 0.0f};
      return {SizeType::Pixels, val};
    }
  };

  enum class FlexDirection { Row, Column };
  enum class JustifyContent { FlexStart, FlexEnd, Center, SpaceBetween };
  enum class AlignItems { FlexStart, FlexEnd, Center };

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
    float& mainSize(FlexDirection dir)
    {
      return (dir == FlexDirection::Row) ? width : height;
    }
    float& crossSize(FlexDirection dir)
    {
      return (dir == FlexDirection::Row) ? height : width;
    }
    float& mainPos(FlexDirection dir)
    {
      return (dir == FlexDirection::Row) ? x : y;
    }
    float& crossPos(FlexDirection dir)
    {
      return (dir == FlexDirection::Row) ? y : x;
    }

    float mainSize(FlexDirection dir) const
    {
      return (dir == FlexDirection::Row) ? width : height;
    }
    float crossSize(FlexDirection dir) const
    {
      return (dir == FlexDirection::Row) ? height : width;
    }
    float mainPos(FlexDirection dir) const
    {
      return (dir == FlexDirection::Row) ? x : y;
    }
    float crossPos(FlexDirection dir) const
    {
      return (dir == FlexDirection::Row) ? y : x;
    }
  };
  std::ostream& operator<<(std::ostream& os, const Size& size);

  struct RLNode;
}
#endif
