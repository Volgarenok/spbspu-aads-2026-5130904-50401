#include "utils.hpp"
#include <iomanip>
#include <sstream>
#include "node-properties.hpp"
#include "node.hpp"
namespace rl {
  namespace config {
    const float DEFAULT_WIDTH = 1920.0f;
    const float DEFAULT_HEIGHT = 1080.0f;
    const char* const DEFAULT_LAYOUT_DIR = "layouts/";
  }

  std::string generateLayoutName(float width, float height)
  {
    std::stringstream ss;
    ss << "layout_" << std::fixed << std::setprecision(0) << width << "x"
       << height;
    return ss.str();
  }

  std::string generateLayoutPath(const std::string& layoutName)
  {
    return std::string(config::DEFAULT_LAYOUT_DIR) + layoutName + ".rl";
  }
  void exportToFlatHtml(std::ostream& os, const RLNode& node)
  {
    os << "<div style=\"" << "position: absolute; " << "left: " << node.box.x
       << "px; " << "top: " << node.box.y << "px; "
       << "width: " << node.box.width << "px; " << "height: " << node.box.height
       << "px; " << "border: 2px solid "
       << (node.flexDirection == FlexDirection::Row ? "blue" : "green") << "; "
       << "box-sizing: border-box;\">"
       << "<span style='background: white; font-size: 12px; position: "
          "absolute;'>"
       << node.id << "</span>";

    for (size_t i = 0; i < node.children.getSize(); ++i) {
      if (node.children[i]) {
        exportToFlatHtml(os, *node.children[i]);
      }
    }

    os << "</div>\n";
  }

}
