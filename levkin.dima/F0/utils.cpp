#include "utils.hpp"
#include <iomanip>
#include <sstream>

namespace rl {
  namespace config {
    const float DEFAULT_WIDTH = 1920.0f;
    const float DEFAULT_HEIGHT = 1080.0f;
    const char* const DEFAULT_LAYOUT_DIR = "layouts/";
  } // namespace config

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

}
