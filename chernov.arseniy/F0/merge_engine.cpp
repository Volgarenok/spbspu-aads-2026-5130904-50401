#include "merge_engine.hpp"
#include <iostream>
#include "tree.hpp"

std::string chernov::detail::formatPersonDisplay(const chernov::Person & p)
{
  std::string s = p.getSurname();
  if (s.empty())
    s = "-";
  std::string n = p.getName();
  if (n.empty())
    n = "-";
  std::string patr = p.getPatronymic();
  if (patr.empty())
    patr = "-";
  return s + " " + n + " " + patr;
}

bool chernov::detail::visitContains(const chernov::Vector< std::pair< std::string, std::string > > & visited,
  const std::string & idA,
  const std::string & idB)
{
  for (size_t i = 0; i < visited.getSize(); ++i) {
    if (visited[i].first == idA && visited[i].second == idB) {
      return true;
    }
  }
  return false;
}

bool chernov::detail::fieldsMatch(const chernov::Person & a, const chernov::Person & b)
{
  if (a.getGender() != b.getGender())
    return false;

  if (!a.getSurname().empty() && !b.getSurname().empty() && a.getSurname() != b.getSurname())
    return false;
  if (!a.getName().empty() && !b.getName().empty() && a.getName() != b.getName())
    return false;
  if (!a.getPatronymic().empty() && !b.getPatronymic().empty() && a.getPatronymic() != b.getPatronymic())
    return false;

  if (!a.getBirthDate().empty() && !b.getBirthDate().empty() && a.getBirthDate() != b.getBirthDate())
    return false;
  if (!a.getDeathDate().empty() && !b.getDeathDate().empty() && a.getDeathDate() != b.getDeathDate())
    return false;

  return true;
}

bool chernov::detail::areMatching(const std::string & idA,
  const std::string & idB,
  const Tree & treeA,
  const Tree & treeB,
  Vector< std::pair< std::string, std::string > > & visited)
{
  if (detail::visitContains(visited, idA, idB)) {
    return false;
  }
  visited.pushBack(std::make_pair(idA, idB));

  const Person * a = treeA.findPerson(idA);
  const Person * b = treeB.findPerson(idB);
  if (!a || !b) {
    return false;
  }

  if (!detail::fieldsMatch(*a, *b)) {
    return false;
  }

  const auto & parentsA = a->getParents();
  const auto & parentsB = b->getParents();

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
    if (!pA || !pB)
      continue;
    out << "<  " << pA->getId() << " (" << nameA << ") ~= " << pB->getId() << " (" << nameB << ")>\n";
    out << "<  [" << detail::formatPersonDisplay(*pA) << "] ~= [" << detail::formatPersonDisplay(*pB) << "]>\n";
  }
}
