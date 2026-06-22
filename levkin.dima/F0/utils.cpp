#include "utils.hpp"
#include <iomanip>
#include <sstream>
#include "node-properties.hpp"
#include "node.hpp"
#include "db.hpp"
#include <fstream>

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
  void exportNodeToHtml(std::ostream& os, const RLNode& node)
  {
    std::string borderColor =
        (node.flexDirection == FlexDirection::Row) ? "blue" : "green";

    os << "<div style=\"" << "position: absolute; " << "left: " << node.box.x
       << "px; " << "top: " << node.box.y << "px; "
       << "width: " << node.box.width << "px; " << "height: " << node.box.height
       << "px; " << "border: 2px solid " << borderColor << "; "
       << "box-sizing: border-box; "
       << "background-color: rgba(0, 0, 0, 0.02);\">\n"
       << "<span style=\"" << "position: absolute; " << "background: white; "
       << "font-family: monospace; " << "font-size: 11px; "
       << "padding: 1px 3px; " << "border: 1px solid #ccc;\">" << node.id
       << "</span>\n"
       << "</div>\n";

    for (size_t i = 0; i < node.children.getSize(); ++i) {
      if (node.children[i]) {
        exportNodeToHtml(os, *node.children[i]);
      }
    }
  }

  void saveVisualTree(const RLRootNode* rootNode, const std::string& filename)
  {
    if (!rootNode)
      return;
    std::ofstream file(filename);
    if (!file.is_open())
      return;

    file << "<!DOCTYPE html>\n<html>\n<head>\n"
         << "<title>Layout Debug</title>\n"
         << "</head>\n<body style=\"margin: 0; padding: 20px; background: "
            "#fafafa;\">\n"
         << "<h3>Layout Engine Debug View (" << rootNode->name << ")</h3>\n"
         << "<div style=\"position: relative; width: "
         << rootNode->root.box.width << "px; "
         << "height: " << rootNode->root.box.height << "px; background: white; "
         << "box-shadow: 0 4px 10px rgba(0,0,0,0.1); border: 1px solid "
            "#ddd;\">\n";

    exportNodeToHtml(file, rootNode->root);

    file << "</div>\n</body>\n</html>\n";
  }
}
