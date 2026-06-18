#include "relationship.hpp"
#include <string>
#include "tree.hpp"
#include <queue.hpp>
#include <stack.hpp>

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

chernov::Vector< chernov::detail::Relative > chernov::detail::findRelatives(
  const Tree & tree, const std::string & id, int maxDepth)
{
  Vector< Relative > result;
  const Person * start = tree.findPerson(id);
  if (!start) {
    return result;
  }

  struct BfsItem {
    std::string personId;
    int depth;
    std::string relation;
  };

  Queue< BfsItem > queue;

  for (size_t i = 0; i < start->getParentsCount(); ++i) {
    BfsItem item;
    item.personId = start->getParents()[i];
    item.depth = 1;
    item.relation = "parent";
    queue.push(item);
  }
  for (size_t i = 0; i < start->getChildrenCount(); ++i) {
    BfsItem item;
    item.personId = start->getChildren()[i];
    item.depth = 1;
    item.relation = "child";
    queue.push(item);
  }
  if (start->hasSpouse()) {
    BfsItem item;
    item.personId = start->getSpouse();
    item.depth = 1;
    item.relation = "spouse";
    queue.push(item);
  }

  Vector< std::string > visited;
  visited.pushBack(id);

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
    if (current.depth == 1) {
      rel.relation = current.relation;
    } else {
      rel.relation = "";
    }
    result.pushBack(rel);

    if (maxDepth == -1 || current.depth < maxDepth) {
      const Person * p = tree.findPerson(current.personId);
      if (p) {
        for (size_t i = 0; i < p->getParentsCount(); ++i) {
          BfsItem next;
          next.personId = p->getParents()[i];
          next.depth = current.depth + 1;
          next.relation = "";
          queue.push(next);
        }
        for (size_t i = 0; i < p->getChildrenCount(); ++i) {
          BfsItem next;
          next.personId = p->getChildren()[i];
          next.depth = current.depth + 1;
          next.relation = "";
          queue.push(next);
        }
        if (p->hasSpouse()) {
          BfsItem next;
          next.personId = p->getSpouse();
          next.depth = current.depth + 1;
          next.relation = "";
          queue.push(next);
        }
      }
    }
  }
  return result;
}
