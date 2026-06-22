#include "db.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace rl {
  static std::string dirToString(FlexDirection d)
  {
    return d == FlexDirection::Column ? "Column" : "Row";
  }
  static FlexDirection stringToDir(const std::string& s)
  {
    return s == "Column" ? FlexDirection::Column : FlexDirection::Row;
  }
  static std::string justifyToString(JustifyContent j)
  {
    if (j == JustifyContent::FlexEnd)
      return "FlexEnd";
    if (j == JustifyContent::Center)
      return "Center";
    if (j == JustifyContent::SpaceBetween)
      return "SpaceBetween";
    return "FlexStart";
  }
  static JustifyContent stringToJustify(const std::string& s)
  {
    if (s == "FlexEnd")
      return JustifyContent::FlexEnd;
    if (s == "Center")
      return JustifyContent::Center;
    if (s == "SpaceBetween")
      return JustifyContent::SpaceBetween;
    return JustifyContent::FlexStart;
  }

  static std::string alignToString(AlignItems a)
  {
    if (a == AlignItems::FlexEnd)
      return "FlexEnd";
    if (a == AlignItems::Center)
      return "Center";
    return "FlexStart";
  }
  static AlignItems stringToAlign(const std::string& s)
  {
    if (s == "FlexEnd")
      return AlignItems::FlexEnd;
    if (s == "Center")
      return AlignItems::Center;
    return AlignItems::FlexStart;
  }

  static void serializeNode(
      std::ostream& os, const RLNode& node, const std::string& parentId)
  {
    os << parentId << " " << node.id << " " << node.width << " " << node.height
       << " " << dirToString(node.flexDirection) << " "
       << justifyToString(node.justify) << " " << alignToString(node.align)
       << "\n";

    for (size_t i = 0; i < node.children.getSize(); ++i) {
      if (node.children[i]) {
        serializeNode(os, *node.children[i], node.id);
      }
    }
  }

  bool RootDB::loadDatabase(const std::string& dbPath)
  {
    std::ifstream file(dbPath);
    if (!file.is_open()) {
      return false;
    }

    storage = Map< std::string, std::unique_ptr< RLRootNode > >();
    selected = nullptr;

    std::string name, path;
    while (file >> name >> path) {
      auto layout =
          std::make_unique< RLRootNode >(name, path, 1920.0f, 1080.0f);
      storage.add(name, std::move(layout));
    }
    return true;
  }

  bool RootDB::saveDatabase(const std::string& dbPath)
  {
    std::ofstream file(dbPath);
    if (!file.is_open()) {
      return false;
    }

    for (auto it = storage.begin(); it != storage.end(); ++it) {
      file << it.key() << " " << it.value()->filePath << "\n";
    }
    return true;
  }

  bool RootDB::loadLayout(const std::string& name)
  {
    if (!storage.has(name)) {
      return false;
    }

    RLRootNode* layout = storage.get(name).get();
    std::ifstream file(layout->filePath);
    if (!file.is_open()) {
      return false;
    }

    layout->root.clearChildren();
    layout->mapOfNodes = Map< std::string, RLNode* >();

    std::string line;
    bool isRootHandled = false;

    while (std::getline(file, line)) {
      if (line.empty())
        continue;
      std::stringstream ss(line);

      std::string parentId, id, dirStr, justifyStr, alignStr;
      float w, h;

      if (!(ss >> parentId >> id >> w >> h >> dirStr >> justifyStr >>
            alignStr)) {
        continue;
      }

      if (parentId == "null" && !isRootHandled) {
        layout->root.id = id;
        layout->root.width = Size::fromFloat(w);
        layout->root.height = Size::fromFloat(h);
        layout->root.flexDirection = stringToDir(dirStr);
        layout->root.justify = stringToJustify(justifyStr);
        layout->root.align = stringToAlign(alignStr);

        layout->mapOfNodes.add(id, &layout->root);
        isRootHandled = true;
      } else {
        if (!layout->mapOfNodes.has(parentId)) {
          std::cerr << "Error: Parent '" << parentId << "' not found for node '"
                    << id << "'\n";
          continue;
        }

        RLNode* parentNode = layout->mapOfNodes.get(parentId);

        auto child = std::make_unique< RLNode >(id, w, h, stringToDir(dirStr));
        child->justify = stringToJustify(justifyStr);
        child->align = stringToAlign(alignStr);

        RLNode* childPtr = child.get();
        parentNode->addChild(std::move(child));

        layout->mapOfNodes.add(id, childPtr);
      }
    }

    selected = layout;
    return true;
  }

  bool RootDB::saveActiveLayout()
  {
    if (!selected) {
      return false;
    }

    std::ofstream file(selected->filePath);
    if (!file.is_open()) {
      return false;
    }

    serializeNode(file, selected->root, "null");
    return true;
  }

  void RootDB::createNewLayout(
      const std::string& name, const std::string& path, float w, float h)
  {
    auto layout = std::make_unique< RLRootNode >(name, path, w, h);
    selected = layout.get();
    storage.add(name, std::move(layout));
  }

  void RootDB::selectLayout(const std::string& name)
  {
    if (storage.has(name)) {
      selected = storage.get(name).get();
    }
  }

  RLRootNode* RootDB::getActive() const { return selected; }

  bool RootDB::hasLayout(const std::string& name) const
  {
    return storage.has(name);
  }

  static void measureNode(RLNode& node)
  {
    for (size_t i = 0; i < node.children.getSize(); ++i) {
      if (node.children[i]) {
        measureNode(*node.children[i]);
      }
    }

    node.box.width =
        (node.width.type == SizeType::Pixels) ? node.width.value : 0.0f;
    node.box.height =
        (node.height.type == SizeType::Pixels) ? node.height.value : 0.0f;

    if (node.children.getSize() == 0)
      return;

    float childrenMainSum = 0.0f;
    float childrenCrossMax = 0.0f;
    const FlexDirection dir = node.flexDirection;

    for (size_t i = 0; i < node.children.getSize(); ++i) {
      if (!node.children[i]) {
        continue;
      }
      const auto& childBox = node.children[i]->box;

      childrenMainSum += childBox.mainSize(dir);
      if (childBox.crossSize(dir) > childrenCrossMax) {
        childrenCrossMax = childBox.crossSize(dir);
      }
    }

    if (node.width.type == SizeType::Auto) {
      node.box.width =
          (dir == FlexDirection::Row) ? childrenMainSum : childrenCrossMax;
    }
    if (node.height.type == SizeType::Auto) {
      node.box.height =
          (dir == FlexDirection::Row) ? childrenCrossMax : childrenMainSum;
    }
  }

  static void arrangeNode(RLNode& node)
  {
    if (node.children.getSize() == 0)
      return;

    const FlexDirection dir = node.flexDirection;
    const float mainParentSize = node.box.mainSize(dir);

    float totalChildrenMainSize = 0.0f;
    size_t validChildren = 0;

    for (size_t i = 0; i < node.children.getSize(); ++i) {
      if (!node.children[i])
        continue;
      totalChildrenMainSize += node.children[i]->box.mainSize(dir);
      validChildren++;
    }

    if (validChildren == 0)
      return;

    float freeSpace = mainParentSize - totalChildrenMainSize;
    float currentMainPos = 0.0f;
    float gap = 0.0f;

    if (node.justify == JustifyContent::FlexEnd) {
      currentMainPos = freeSpace;
    } else if (node.justify == JustifyContent::Center) {
      currentMainPos = freeSpace / 2.0f;
    } else if (
        node.justify == JustifyContent::SpaceBetween && validChildren > 1) {
      gap = freeSpace / static_cast< float >(validChildren - 1);
    }

    for (size_t i = 0; i < node.children.getSize(); ++i) {
      if (!node.children[i])
        continue;
      auto& child = *node.children[i];
      child.box.mainPos(dir) = node.box.mainPos(dir) + currentMainPos;
      float crossParentSize = node.box.crossSize(dir);
      float crossChildSize = child.box.crossSize(dir);
      float currentCrossPos = 0.0f;

      if (node.align == AlignItems::FlexEnd) {
        currentCrossPos = crossParentSize - crossChildSize;
      } else if (node.align == AlignItems::Center) {
        currentCrossPos = (crossParentSize - crossChildSize) / 2.0f;
      }
      child.box.crossPos(dir) = node.box.crossPos(dir) + currentCrossPos;
      currentMainPos += child.box.mainSize(dir) + gap;

      arrangeNode(child);
    }
  }
  void RootDB::calculateLayout()
  {
    if (!selected)
      return;

    selected->root.box.x = 0.0f;
    selected->root.box.y = 0.0f;

    if (selected->root.width.type == SizeType::Auto) {
      selected->root.box.width = 1920.0f;
    } else {
      selected->root.box.width = selected->root.width.value;
    }

    if (selected->root.height.type == SizeType::Auto) {
      selected->root.box.height = 1080.0f;
    } else {
      selected->root.box.height = selected->root.height.value;
    }

    measureNode(selected->root);
    arrangeNode(selected->root);
  }
}
