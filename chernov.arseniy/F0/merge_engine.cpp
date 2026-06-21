#include "merge_engine.hpp"
#include <iostream>
#include "date_utils.hpp"
#include "tree.hpp"

std::string chernov::detail::formatPersonDisplay(const Person & p)
{
  std::string s = p.getSurname();
  if (s.empty()) {
    s = "-";
  }
  std::string n = p.getName();
  if (n.empty()) {
    n = "-";
  }
  std::string patr = p.getPatronymic();
  if (patr.empty()) {
    patr = "-";
  }
  return s + " " + n + " " + patr;
}

bool chernov::detail::visitContains(
  const Vector< std::pair< std::string, std::string > > & visited, const std::string & idA, const std::string & idB)
{
  for (size_t i = 0; i < visited.getSize(); ++i) {
    if (visited[i].first == idA && visited[i].second == idB) {
      return true;
    }
  }
  return false;
}

bool chernov::detail::fieldsMatch(const Person & a, const Person & b)
{
  if (a.getGender() != b.getGender()) {
    return false;
  }
  if (!a.getSurname().empty() && !b.getSurname().empty() && a.getSurname() != b.getSurname()) {
    return false;
  }
  if (!a.getName().empty() && !b.getName().empty() && a.getName() != b.getName()) {
    return false;
  }
  if (!a.getPatronymic().empty() && !b.getPatronymic().empty() && a.getPatronymic() != b.getPatronymic()) {
    return false;
  }
  if (!a.getBirthDate().empty() && !b.getBirthDate().empty() && a.getBirthDate() != b.getBirthDate()) {
    return false;
  }
  if (!a.getDeathDate().empty() && !b.getDeathDate().empty() && a.getDeathDate() != b.getDeathDate()) {
    return false;
  }
  return true;
}

bool chernov::detail::areMatching(const std::string & idA,
  const std::string & idB,
  const Tree & treeA,
  const Tree & treeB,
  Vector< std::pair< std::string, std::string > > & visited)
{
  if (visitContains(visited, idA, idB)) {
    return true;
  }
  visited.pushBack(std::make_pair(idA, idB));

  const Person * a = treeA.findPerson(idA);
  const Person * b = treeB.findPerson(idB);
  if ((!a) || (!b)) {
    return false;
  }

  if (!fieldsMatch(*a, *b)) {
    return false;
  }

  const Vector< std::string > & parentsA = a->getParents();
  const Vector< std::string > & parentsB = b->getParents();

  if (parentsA.getSize() > 0 && parentsB.getSize() > 0) {
    bool matchedAll = true;
    for (size_t i = 0; i < parentsA.getSize(); ++i) {
      bool found = false;
      for (size_t j = 0; j < parentsB.getSize(); ++j) {
        if (areMatching(parentsA[i], parentsB[j], treeA, treeB, visited)) {
          found = true;
          break;
        }
      }
      if (!found) {
        matchedAll = false;
        break;
      }
    }
    if (!matchedAll) {
      return false;
    }
  }

  if (a->hasSpouse() && b->hasSpouse()) {
    const std::string & spouseA = a->getSpouse();
    const std::string & spouseB = b->getSpouse();
    if (!areMatching(spouseA, spouseB, treeA, treeB, visited)) {
      return false;
    }
  }

  return true;
}

void chernov::detail::compareTrees(
  const Tree & treeA, const Tree & treeB, const std::string & nameA, const std::string & nameB, std::ostream & out)
{
  Vector< std::string > idsA = treeA.getAllPersonIds();
  Vector< std::pair< std::string, std::string > > matches;

  for (size_t i = 0; i < idsA.getSize(); ++i) {
    const std::string & idA = idsA[i];
    Vector< std::string > idsB = treeB.getAllPersonIds();
    for (size_t j = 0; j < idsB.getSize(); ++j) {
      const std::string & idB = idsB[j];
      Vector< std::pair< std::string, std::string > > visited;
      if (areMatching(idA, idB, treeA, treeB, visited)) {
        matches.pushBack(std::make_pair(idA, idB));
      }
    }
  }

  out << "<COMPARISON RESULTS:>\n";
  for (size_t i = 0; i < matches.getSize(); ++i) {
    const Person * pA = treeA.findPerson(matches[i].first);
    const Person * pB = treeB.findPerson(matches[i].second);
    if ((!pA) || (!pB)) {
      continue;
    }
    out << "<  " << pA->getId() << " (" << nameA << ") ~= " << pB->getId() << " (" << nameB << ")>\n";
    out << "<  [" << formatPersonDisplay(*pA) << "] ~= [" << formatPersonDisplay(*pB) << "]>\n";
  }
}

chernov::detail::MergeContext::MergeContext(Tree & mTree, const Tree & a, const Tree & b):
  mergedTree(mTree),
  treeA(a),
  treeB(b),
  pairMap(16),
  copyMapA(16),
  copyMapB(16)
{}

std::string chernov::detail::MergeContext::makeKey(const std::string & idA, const std::string & idB)
{
  return idA + "|" + idB;
}

std::string chernov::detail::MergeContext::mergePair(
  const std::string & idA, const std::string & idB, std::string & error)
{
  std::string key = makeKey(idA, idB);
  if (pairMap.has(key)) {
    return pairMap.at(key);
  }

  const Person * a = treeA.findPerson(idA);
  const Person * b = treeB.findPerson(idB);
  if ((!a) || (!b)) {
    error = "person not found";
    return "";
  }

  Vector< std::pair< std::string, std::string > > visited;
  if (!areMatching(idA, idB, treeA, treeB, visited)) {
    error = "Merge conflict! Persons are not matching.";
    return "";
  }

  std::string newId = mergedTree.addPersonCopy(*a);
  if (newId.empty()) {
    error = "failed to create person";
    return "";
  }
  pairMap.add(key, newId);

  const Vector< std::string > & parentsA = a->getParents();
  const Vector< std::string > & parentsB = b->getParents();
  Vector< bool > usedB(parentsB.getSize(), false);

  for (size_t i = 0; i < parentsA.getSize(); ++i) {
    const std::string & pA = parentsA[i];
    const Person * parentA = treeA.findPerson(pA);
    if (!parentA) {
      continue;
    }
    bool matched = false;
    for (size_t j = 0; j < parentsB.getSize(); ++j) {
      if (usedB[j]) {
        continue;
      }
      const std::string & pB = parentsB[j];
      const Person * parentB = treeB.findPerson(pB);
      if (!parentB) {
        continue;
      }
      if (parentA->getGender() != parentB->getGender()) {
        continue;
      }
      Vector< std::pair< std::string, std::string > > vis;
      if (areMatching(pA, pB, treeA, treeB, vis)) {
        std::string mergedParent = mergePair(pA, pB, error);
        if (mergedParent.empty()) {
          return "";
        }
        if (!mergedTree.addParent(newId, mergedParent, error)) {
          error = "failed to add merged parent";
          return "";
        }
        usedB[j] = true;
        matched = true;
        break;
      }
    }
    if (!matched) {
      std::string newParent = copyFromA(pA, error);
      if (newParent.empty()) {
        return "";
      }
      if (!mergedTree.addParent(newId, newParent, error)) {
        return "";
      }
    }
  }

  for (size_t j = 0; j < parentsB.getSize(); ++j) {
    if (usedB[j]) {
      continue;
    }
    const std::string & pB = parentsB[j];
    const Person * parentB = treeB.findPerson(pB);
    if (!parentB) {
      continue;
    }
    const Person * mergedPersonPtr = mergedTree.findPerson(newId);
    if (mergedPersonPtr) {
      const Vector< std::string > & existingParents = mergedPersonPtr->getParents();
      bool conflict = false;
      for (size_t k = 0; k < existingParents.getSize(); ++k) {
        const Person * ep = mergedTree.findPerson(existingParents[k]);
        if (ep && ep->getGender() == parentB->getGender() && parentB->getGender() != "Unknown") {
          conflict = true;
          break;
        }
      }
      if (conflict) {
        error = "Merge conflict! Spouses mismatch. Cannot merge.";
        return "";
      }
    }
    std::string newParent = copyFromB(pB, error);
    if (newParent.empty()) {
      return "";
    }
    if (!mergedTree.addParent(newId, newParent, error)) {
      return "";
    }
  }

  if (mergedTree.findPerson(newId) && mergedTree.findPerson(newId)->getParentsCount() > 2) {
    error = "Merge conflict! Too many parents.";
    return "";
  }

  if (a->hasSpouse() && b->hasSpouse()) {
    Vector< std::pair< std::string, std::string > > vis;
    if (areMatching(a->getSpouse(), b->getSpouse(), treeA, treeB, vis)) {
      std::string mergedSpouse = mergePair(a->getSpouse(), b->getSpouse(), error);
      if (mergedSpouse.empty()) {
        return "";
      }
      if (!mergedTree.addSpouse(newId, mergedSpouse, error)) {
        error = "Merge conflict! Spouse already set.";
        return "";
      }
    } else {
      error = "Merge conflict! Spouses mismatch. Cannot merge.";
      return "";
    }
  } else if (a->hasSpouse()) {
    std::string spouseNew = copyFromA(a->getSpouse(), error);
    if (spouseNew.empty()) {
      return "";
    }
    if (!mergedTree.addSpouse(newId, spouseNew, error)) {
      return "";
    }
  } else if (b->hasSpouse()) {
    std::string spouseNew = copyFromB(b->getSpouse(), error);
    if (spouseNew.empty()) {
      return "";
    }
    if (!mergedTree.addSpouse(newId, spouseNew, error)) {
      return "";
    }
  }

  const Vector< std::string > & childrenA = a->getChildren();
  const Vector< std::string > & childrenB = b->getChildren();
  Vector< bool > usedChildB(childrenB.getSize(), false);

  for (size_t i = 0; i < childrenA.getSize(); ++i) {
    const std::string & cA = childrenA[i];
    bool matched = false;
    for (size_t j = 0; j < childrenB.getSize(); ++j) {
      if (usedChildB[j]) {
        continue;
      }
      const std::string & cB = childrenB[j];
      Vector< std::pair< std::string, std::string > > vis;
      if (areMatching(cA, cB, treeA, treeB, vis)) {
        std::string mergedChild = mergePair(cA, cB, error);
        if (mergedChild.empty()) {
          return "";
        }
        if (!mergedTree.addParent(mergedChild, newId, error)) {
          return "";
        }
        usedChildB[j] = true;
        matched = true;
        break;
      }
    }
    if (!matched) {
      std::string newChild = copyFromA(cA, error);
      if (newChild.empty()) {
        return "";
      }
      if (!mergedTree.addParent(newChild, newId, error)) {
        return "";
      }
    }
  }
  for (size_t j = 0; j < childrenB.getSize(); ++j) {
    if (!usedChildB[j]) {
      std::string newChild = copyFromB(childrenB[j], error);
      if (newChild.empty()) {
        return "";
      }
      if (!mergedTree.addParent(newChild, newId, error)) {
        return "";
      }
    }
  }

  return newId;
}

std::string chernov::detail::MergeContext::copyFromA(const std::string & idA, std::string & error)
{
  if (copyMapA.has(idA)) {
    return copyMapA.at(idA);
  }
  const Person * a = treeA.findPerson(idA);
  if (!a) {
    return "";
  }
  std::string newId = mergedTree.addPersonCopy(*a);
  if (newId.empty()) {
    return "";
  }
  copyMapA.add(idA, newId);

  for (size_t i = 0; i < a->getParentsCount(); ++i) {
    std::string parentNew = copyFromA(a->getParents()[i], error);
    if (parentNew.empty()) {
      return "";
    }
    if (!mergedTree.addParent(newId, parentNew, error)) {
      return "";
    }
  }
  if (a->hasSpouse()) {
    std::string spouseNew = copyFromA(a->getSpouse(), error);
    if (spouseNew.empty()) {
      return "";
    }
    if (!mergedTree.addSpouse(newId, spouseNew, error)) {
      return "";
    }
  }
  for (size_t i = 0; i < a->getChildrenCount(); ++i) {
    std::string childNew = copyFromA(a->getChildren()[i], error);
    if (childNew.empty()) {
      return "";
    }
    if (!mergedTree.addParent(childNew, newId, error)) {
      return "";
    }
  }
  return newId;
}

std::string chernov::detail::MergeContext::copyFromB(const std::string & idB, std::string & error)
{
  if (copyMapB.has(idB)) {
    return copyMapB.at(idB);
  }
  const Person * b = treeB.findPerson(idB);
  if (!b) {
    return "";
  }
  std::string newId = mergedTree.addPersonCopy(*b);
  if (newId.empty()) {
    return "";
  }
  copyMapB.add(idB, newId);

  for (size_t i = 0; i < b->getParentsCount(); ++i) {
    std::string parentNew = copyFromB(b->getParents()[i], error);
    if (parentNew.empty()) {
      return "";
    }
    if (!mergedTree.addParent(newId, parentNew, error)) {
      return "";
    }
  }
  if (b->hasSpouse()) {
    std::string spouseNew = copyFromB(b->getSpouse(), error);
    if (spouseNew.empty()) {
      return "";
    }
    if (!mergedTree.addSpouse(newId, spouseNew, error)) {
      return "";
    }
  }
  for (size_t i = 0; i < b->getChildrenCount(); ++i) {
    std::string childNew = copyFromB(b->getChildren()[i], error);
    if (childNew.empty()) {
      return "";
    }
    if (!mergedTree.addParent(childNew, newId, error)) {
      return "";
    }
  }
  return newId;
}

bool chernov::detail::mergePersons(const Tree & treeA,
  const std::string & idA,
  const Tree & treeB,
  const std::string & idB,
  Tree & mergedTree,
  std::string & errorMsg)
{
  MergeContext ctx(mergedTree, treeA, treeB);
  std::string newId = ctx.mergePair(idA, idB, errorMsg);
  return !newId.empty();
}
