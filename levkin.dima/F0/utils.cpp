#include "stb_image_write.hpp"
#include "utils.hpp"
#include <iomanip>
#include <sstream>
#include "node-properties.hpp"
#include "node.hpp"
#include "db.hpp"
#include <fstream>
#include "vector.hpp"
namespace rl {
  namespace {
    struct ColorRGB {
      uint8_t r, g, b;
    };
    ColorRGB getBackgroundColorPng(const std::string& id)
    {
      static const ColorRGB BEAUTIFUL_COLORS_RGB[] = {
          {255, 127, 80},  {255, 105, 180}, {255, 99, 71},  {255, 140, 0},
          {255, 215, 0},   {186, 85, 211},  {0, 191, 255},  {0, 250, 154},
          {100, 149, 237}, {220, 20, 60},   {244, 164, 96}, {64, 224, 208},
          {154, 205, 50},  {221, 160, 221}};

      constexpr size_t colorCount =
          sizeof(BEAUTIFUL_COLORS_RGB) / sizeof(BEAUTIFUL_COLORS_RGB[0]);
      size_t colorIdx = (std::hash< std::string >{}(id) + 3) % colorCount;
      return BEAUTIFUL_COLORS_RGB[colorIdx];
    }
    void drawRect(
        stf::Vector< uint8_t >& buffer,
        int canvasW,
        int canvasH,
        float rx,
        float ry,
        float rw,
        float rh,
        ColorRGB color)
    {
      int xStart = std::max(0, static_cast< int >(rx));
      int yStart = std::max(0, static_cast< int >(ry));
      int xEnd = std::min(canvasW, static_cast< int >(rx + rw));
      int yEnd = std::min(canvasH, static_cast< int >(ry + rh));

      for (int y = yStart; y < yEnd; ++y) {
        for (int x = xStart; x < xEnd; ++x) {
          size_t pixelIdx =
              (static_cast< size_t >(y) * static_cast< size_t >(canvasW) +
               static_cast< size_t >(x)) *
              3;
          buffer[pixelIdx] = color.r;
          buffer[pixelIdx + 1] = color.g;
          buffer[pixelIdx + 2] = color.b;
        }
      }
    }

    std::string getBackgroundColor(const std::string& id)
    {
      static const char* const BEAUTIFUL_COLORS[] = {
          "coral",          "hotpink",      "tomato",      "darkorange",
          "gold",           "mediumorchid", "deepskyblue", "mediumspringgreen",
          "cornflowerblue", "crimson",      "sandybrown",  "turquoise",
          "yellowgreen",    "plum"};
      constexpr size_t colorCount =
          sizeof(BEAUTIFUL_COLORS) / sizeof(BEAUTIFUL_COLORS[0]);
      size_t colorIdx = (std::hash< std::string >{}(id) + 3) % colorCount;
      return BEAUTIFUL_COLORS[colorIdx];
    }

    void renderNodeToBuffer(
        stf::Vector< uint8_t >& buffer, int w, int h, const RLNode& node)
    {
      ColorRGB color = getBackgroundColorPng(node.id);
      drawRect(
          buffer, w, h, node.box.x, node.box.y, node.box.width, node.box.height,
          color);

      for (size_t i = 0; i < node.children.getSize(); ++i) {
        if (node.children[i]) {
          renderNodeToBuffer(buffer, w, h, *node.children[i]);
        }
      }
    }

    std::string getContainerStyle(const RLNode& node)
    {
      std::string color = getBackgroundColor(node.id);
      return "position: absolute; "
             "left: " +
             std::to_string(node.box.x) +
             "px; "
             "top: " +
             std::to_string(node.box.y) +
             "px; "
             "width: " +
             std::to_string(node.box.width) +
             "px; "
             "height: " +
             std::to_string(node.box.height) +
             "px; "
             "box-sizing: border-box; "
             "background-color: " +
             color + ";";
    }

    const char* getLabelStyle()
    {
      return "position: absolute; "
             "background: rgba(255, 255, 255, 0.85); "
             "font-family: monospace; "
             "font-size: 11px; "
             "font-weight: bold; "
             "padding: 1px 4px; "
             "border-right: 1px solid #ccc; "
             "border-bottom: 1px solid #ccc;";
    }

    void renderSingleNodeHtml(std::ostream& os, const RLNode& node)
    {
      os << "<div style=\"" << getContainerStyle(node) << "\">\n"
         << "<span style=\"" << getLabelStyle() << "\">" << node.id
         << "</span>\n"
         << "</div>\n";
    }
  }
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
    renderSingleNodeHtml(os, node);

    for (size_t i = 0; i < node.children.getSize(); ++i) {
      if (node.children[i]) {
        exportNodeToHtml(os, *node.children[i]);
      }
    }
  }
  void
  saveVisualTreeToPng(const RLRootNode* rootNode, const std::string& filename)
  {
    if (!rootNode)
      return;

    int width = static_cast< int >(rootNode->root.box.width);
    int height = static_cast< int >(rootNode->root.box.height);

    if (width <= 0 || height <= 0)
      return;

    stuff::Vector< uint8_t > canvasBuffer;
    canvasBuffer.pushBackCount(static_cast< size_t >(width * height * 3), 255);

    renderNodeToBuffer(canvasBuffer, width, height, rootNode->root);

    stbi_write_png(
        filename.c_str(), width, height, 3, &canvasBuffer[0], width * 3);
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
