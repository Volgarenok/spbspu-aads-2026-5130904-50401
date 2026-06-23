#include "relationship.hpp"
#include <string>
#include <queue.hpp>
#include <stack.hpp>
#include "tree.hpp"

chernov::Vector< std::string > chernov::detail::findAncestors(const Tree & tree, const std::string & id, int maxDepth)
{
  Vector< std::string > result;
  const Person * person = tree.findPerson(id);
  if (!person) {
    return result;
  }

  struct StackItem {
    std::string personId;
    int depth;
  };

  Stack< StackItem > stack;
  for (size_t i = 0; i < person->getParentsCount(); ++i) {
    StackItem item;
    item.personId = person->getParents()[i];
    item.depth = 1;
    stack.push(item);
  }

  while (!stack.empty()) {
    StackItem current = stack.top();
    stack.pop();
    result.pushBack(current.personId);
    if (maxDepth == -1 || current.depth < maxDepth) {
      const Person * parent = tree.findPerson(current.personId);
      if (parent) {
        for (size_t i = 0; i < parent->getParentsCount(); ++i) {
          StackItem next;
          next.personId = parent->getParents()[i];
          next.depth = current.depth + 1;
          stack.push(next);
        }
      }
    }
  }
  return result;
}

chernov::Vector< std::string > chernov::detail::findDescendants(const Tree & tree, const std::string & id, int maxDepth)
{
  Vector< std::string > result;
  const Person * person = tree.findPerson(id);
  if (!person) {
    return result;
  }

  struct QueueItem {
    std::string personId;
    int depth;
  };

  Queue< QueueItem > queue;
  for (size_t i = 0; i < person->getChildrenCount(); ++i) {
    QueueItem item;
    item.personId = person->getChildren()[i];
    item.depth = 1;
    queue.push(item);
  }

  while (!queue.empty()) {
    QueueItem current = queue.front();
    queue.pop();
    result.pushBack(current.personId);
    if (maxDepth == -1 || current.depth < maxDepth) {
      const Person * child = tree.findPerson(current.personId);
      if (child) {
        for (size_t i = 0; i < child->getChildrenCount(); ++i) {
          QueueItem next;
          next.personId = child->getChildren()[i];
          next.depth = current.depth + 1;
          queue.push(next);
        }
      }
    }
  }
  return result;
}

chernov::Vector< chernov::detail::Relative >
  chernov::detail::findRelatives(const Tree & tree, const std::string & id, int maxDepth)
{
  Vector< Relative > result;
  const Person * start = tree.findPerson(id);
  if (!start) {
    return result;
  }

  struct BfsItem {
    std::string personId;
    int depth;
    bool spouseLink;
  };

  Queue< BfsItem > queue;
  Vector< std::string > visited;
  visited.pushBack(id);

  for (size_t i = 0; i < start->getParentsCount(); ++i) {
    BfsItem item;
    item.personId = start->getParents()[i];
    item.depth = 1;
    item.spouseLink = false;
    queue.push(item);
  }
  for (size_t i = 0; i < start->getChildrenCount(); ++i) {
    BfsItem item;
    item.personId = start->getChildren()[i];
    item.depth = 1;
    item.spouseLink = false;
    queue.push(item);
  }
  if (start->hasSpouse()) {
    BfsItem item;
    item.personId = start->getSpouse();
    item.depth = 1;
    item.spouseLink = true;
    queue.push(item);
  }

  while (!queue.empty()) {
    BfsItem current = queue.front();
    queue.pop();

    bool alreadyVisited = false;
    for (size_t i = 0; i < visited.getSize(); ++i) {
      if (visited[i] == current.personId) {
        alreadyVisited = true;
        break;
      }
    }
    if (alreadyVisited) {
      continue;
    }
    visited.pushBack(current.personId);

    Relative rel;
    rel.id = current.personId;
    if (current.spouseLink) {
      rel.relation = "Spouse";
    } else {
      rel.relation = getRelationLabel(tree, current.personId, id);
    }
    result.pushBack(rel);

    if (maxDepth == -1 || current.depth < maxDepth) {
      const Person * p = tree.findPerson(current.personId);
      if (p) {
        for (size_t i = 0; i < p->getParentsCount(); ++i) {
          BfsItem next;
          next.personId = p->getParents()[i];
          next.depth = current.depth + 1;
          next.spouseLink = false;
          queue.push(next);
        }
        for (size_t i = 0; i < p->getChildrenCount(); ++i) {
          BfsItem next;
          next.personId = p->getChildren()[i];
          next.depth = current.depth + 1;
          next.spouseLink = false;
          queue.push(next);
        }
        if (p->hasSpouse()) {
          BfsItem next;
          next.personId = p->getSpouse();
          next.depth = current.depth + 1;
          next.spouseLink = true;
          queue.push(next);
        }
      }
    }
  }
  return result;
}

void chernov::detail::collectAncestorDistances(const Tree & tree, const std::string & id, AncestorDistances & distances)
{
  struct QItem {
    std::string personId;
    int dist;
  };
  Queue< QItem > queue;
  QItem start;
  start.personId = id;
  start.dist = 0;
  queue.push(start);
  distances.add(id, 0);

  while (!queue.empty()) {
    QItem cur = queue.front();
    queue.pop();
    const Person * p = tree.findPerson(cur.personId);
    if (!p) {
      continue;
    }
    for (size_t i = 0; i < p->getParentsCount(); ++i) {
      std::string parentId = p->getParents()[i];
      if (!distances.has(parentId)) {
        distances.add(parentId, cur.dist + 1);
        QItem next;
        next.personId = parentId;
        next.dist = cur.dist + 1;
        queue.push(next);
      }
    }
  }
}

std::string chernov::detail::makeGreatPrefix(int count)
{
  std::string prefix;
  for (int i = 0; i < count; ++i) {
    prefix += "great-";
  }
  return prefix;
}

std::string chernov::detail::getAncestorTitle(const std::string & gender, int upDist)
{
  if (upDist <= 0) {
    return "self";
  }
  if (upDist == 1) {
    if (gender == "Male") {
      return "Father";
    } else if (gender == "Female") {
      return "Mother";
    } else {
      return "Parent";
    }
  }
  std::string greats = makeGreatPrefix(upDist - 2);
  if (gender == "Male") {
    return greats + "Grandfather";
  } else if (gender == "Female") {
    return greats + "Grandmother";
  } else {
    return greats + "Grandparent";
  }
}

std::string chernov::detail::getDescendantTitle(const std::string & gender, int downDist)
{
  if (downDist <= 0) {
    return "self";
  }
  if (downDist == 1) {
    if (gender == "Male") {
      return "Son";
    } else if (gender == "Female") {
      return "Daughter";
    } else {
      return "Child";
    }
  }
  std::string greats = makeGreatPrefix(downDist - 2);
  if (gender == "Male") {
    return greats + "Grandson";
  } else if (gender == "Female") {
    return greats + "Granddaughter";
  } else {
    return greats + "Grandchild";
  }
}

chernov::Vector< std::string >
  chernov::detail::findCommonAncestors(const Tree & tree, const std::string & id1, const std::string & id2)
{
  Vector< std::string > result;
  AncestorDistances map1(16), map2(16);
  collectAncestorDistances(tree, id1, map1);
  collectAncestorDistances(tree, id2, map2);

  for (auto it = map1.begin(); it != map1.end(); ++it) {
    const std::string & ancId = (*it).first;
    if (map2.has(ancId)) {
      result.pushBack(ancId);
    }
  }
  return result;
}

std::string chernov::detail::getRelationship(const Tree & tree, const std::string & id1, const std::string & id2)
{
  if (id1 == id2) {
    return id1 + " is the same person as " + id2;
  }
  const Person * p1 = tree.findPerson(id1);
  const Person * p2 = tree.findPerson(id2);
  if (!p1 || !p2) {
    return "Person not found";
  }

  if (p1->hasSpouse() && p1->getSpouse() == id2) {
    if (p1->getGender() == "Male") {
      return id1 + " is the Husband of " + id2;
    } else if (p1->getGender() == "Female") {
      return id1 + " is the Wife of " + id2;
    } else {
      return id1 + " is the Spouse of " + id2;
    }
  }

  AncestorDistances dist1(16), dist2(16);
  collectAncestorDistances(tree, id1, dist1);
  collectAncestorDistances(tree, id2, dist2);

  int minSum = -1;
  int up1 = -1, up2 = -1;
  for (auto it = dist1.begin(); it != dist1.end(); ++it) {
    const std::string & anc = (*it).first;
    int d1 = (*it).second;
    if (dist2.has(anc)) {
      int d2 = dist2.at(anc);
      int sum = d1 + d2;
      if (minSum == -1 || sum < minSum) {
        minSum = sum;
        up1 = d1;
        up2 = d2;
      }
    }
  }
  if (minSum == -1) {
    return id1 + " and " + id2 + " are not related";
  }

  if (up1 == 0) {
    return id1 + " is the " + getAncestorTitle(p1->getGender(), up2) + " of " + id2;
  }
  if (up2 == 0) {
    return id1 + " is the " + getDescendantTitle(p1->getGender(), up1) + " of " + id2;
  }

  if (up1 == 1 && up2 == 1) {
    if (p1->getGender() == "Male") {
      return id1 + " is the Brother of " + id2;
    } else if (p1->getGender() == "Female") {
      return id1 + " is the Sister of " + id2;
    } else {
      return id1 + " is the Sibling of " + id2;
    }
  }
  if (up1 == 1 && up2 == 2) {
    if (p1->getGender() == "Male") {
      return id1 + " is the Uncle of " + id2;
    } else if (p1->getGender() == "Female") {
      return id1 + " is the Aunt of " + id2;
    } else {
      return id1 + " is the Uncle/Aunt of " + id2;
    }
  }
  if (up1 == 2 && up2 == 1) {
    if (p1->getGender() == "Male") {
      return id1 + " is the Nephew of " + id2;
    } else if (p1->getGender() == "Female") {
      return id1 + " is the Niece of " + id2;
    } else {
      return id1 + " is the Nephew/Niece of " + id2;
    }
  }
  if (up1 == 2 && up2 == 2) {
    if (p1->getGender() == "Male") {
      return id1 + " is the Cousin (male) of " + id2;
    } else if (p1->getGender() == "Female") {
      return id1 + " is the Cousin (female) of " + id2;
    } else {
      return id1 + " is the Cousin of " + id2;
    }
  }
  return id1 + " is a distant relative of " + id2;
}

std::string chernov::detail::getRelationLabel(const Tree & tree, const std::string & from, const std::string & to)
{
  const Person * pFrom = tree.findPerson(from);
  const Person * pTo = tree.findPerson(to);
  if (!pFrom || !pTo) {
    return "";
  }
  if (from == to) {
    return "self";
  }

  if (pFrom->hasSpouse() && pFrom->getSpouse() == to) {
    if (pFrom->getGender() == "Male") {
      return "Husband";
    } else if (pFrom->getGender() == "Female") {
      return "Wife";
    } else {
      return "Spouse";
    }
  }
  if (pTo->hasSpouse() && pTo->getSpouse() == from) {
    if (pFrom->getGender() == "Male") {
      return "Husband";
    } else if (pFrom->getGender() == "Female") {
      return "Wife";
    } else {
      return "Spouse";
    }
  }

  std::string full = getRelationship(tree, from, to);
  std::string pattern = " is the ";
  size_t pos1 = full.find(pattern);
  if (pos1 == std::string::npos) {
    return "";
  }
  pos1 += pattern.size();
  size_t pos2 = full.find(" of ", pos1);
  if (pos2 == std::string::npos) {
    return "";
  }
  return full.substr(pos1, pos2 - pos1);
}
