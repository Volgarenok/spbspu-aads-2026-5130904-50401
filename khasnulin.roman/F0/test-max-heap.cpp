#include "MaxHeap.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <random>
#include <stdexcept>
#include <string>

using namespace khasnulin;

BOOST_AUTO_TEST_SUITE(max_heap_test_suite)

BOOST_AUTO_TEST_CASE(test_empty_creation_positive)
{
  MaxHeap< int, std::less< int > > heap;

  BOOST_CHECK(heap.empty() == true);
  BOOST_CHECK(heap.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_empty_creation_get_top_negative)
{
  MaxHeap< int, std::less< int > > heap;

  BOOST_CHECK_THROW(heap.top(), std::out_of_range);
  BOOST_CHECK_THROW(heap.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_push_element_positive)
{
  MaxHeap< int, std::less< int > > heap;

  heap.push(5);

  BOOST_CHECK(!heap.empty());
  BOOST_CHECK_EQUAL(heap.top(), 5);
  BOOST_CHECK_EQUAL(heap.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_push_decreasing_sequence_positive)
{
  MaxHeap< int, std::less< int > > heap;

  heap.push(5);
  heap.push(4);
  heap.push(3);

  BOOST_CHECK(!heap.empty());
  BOOST_CHECK_EQUAL(heap.top(), 5);
  BOOST_CHECK_EQUAL(heap.size(), 3);

  heap.pop();
  BOOST_CHECK_EQUAL(heap.top(), 4);
  BOOST_CHECK_EQUAL(heap.size(), 2);

  heap.pop();
  BOOST_CHECK_EQUAL(heap.top(), 3);
  BOOST_CHECK_EQUAL(heap.size(), 1);

  heap.pop();
  BOOST_CHECK_EQUAL(heap.size(), 0);
  BOOST_CHECK_THROW(heap.top(), std::out_of_range);
  BOOST_CHECK_THROW(heap.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_push_increasing_sequence_positive)
{
  MaxHeap< int, std::less< int > > heap;

  heap.push(1);
  BOOST_CHECK(!heap.empty());
  BOOST_CHECK_EQUAL(heap.top(), 1);
  BOOST_CHECK_EQUAL(heap.size(), 1);

  heap.push(2);
  BOOST_CHECK_EQUAL(heap.top(), 2);
  BOOST_CHECK_EQUAL(heap.size(), 2);

  heap.push(3);
  BOOST_CHECK_EQUAL(heap.top(), 3);
  BOOST_CHECK_EQUAL(heap.size(), 3);

  heap.push(4);
  BOOST_CHECK_EQUAL(heap.top(), 4);
  BOOST_CHECK_EQUAL(heap.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_push_duplicate_sequence_positive)
{
  MaxHeap< int, std::less< int > > heap;

  heap.push(5);
  BOOST_CHECK(!heap.empty());
  BOOST_CHECK_EQUAL(heap.top(), 5);
  BOOST_CHECK_EQUAL(heap.size(), 1);

  heap.push(5);
  BOOST_CHECK_EQUAL(heap.top(), 5);
  BOOST_CHECK_EQUAL(heap.size(), 2);

  heap.push(5);
  BOOST_CHECK_EQUAL(heap.top(), 5);
  BOOST_CHECK_EQUAL(heap.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_pop_sequence_positive)
{
  MaxHeap< int, std::less< int > > heap;

  heap.push(10);
  heap.push(15);
  BOOST_CHECK(!heap.empty());
  BOOST_CHECK_EQUAL(heap.top(), 15);
  BOOST_CHECK_EQUAL(heap.size(), 2);

  heap.pop();
  BOOST_CHECK_EQUAL(heap.top(), 10);
  BOOST_CHECK_EQUAL(heap.size(), 1);

  heap.push(12);
  BOOST_CHECK_EQUAL(heap.top(), 12);
  BOOST_CHECK_EQUAL(heap.size(), 2);

  heap.pop();
  heap.pop();

  BOOST_CHECK(heap.empty());
  BOOST_CHECK_EQUAL(heap.size(), 0);
}

struct Person
{
  size_t age;
  std::string name;
  Person():
      age(0),
      name("unknown")
  {
  }
  Person(size_t a):
      age(a),
      name("unknown")
  {
  }
  Person(const std::string &n):
      age(0),
      name(n)
  {
  }
  Person(size_t a, const std::string &n):
      age(a),
      name(n)
  {
  }

  bool operator==(const Person &rhs) const
  {
    return age == rhs.age && name == rhs.name;
  }
  bool operator!=(const Person &rhs) const
  {
    return !(*this == rhs);
  }
};

struct PersonComparator
{
  bool operator()(const Person &left, const Person &right) const
  {
    return left.name < right.name;
  };
};

struct PersonInversedComparator
{
  bool operator()(const Person &left, const Person &right) const
  {
    return left.name > right.name;
  };
};

std::ostream &operator<<(std::ostream &out, const Person &rhs)
{
  return out << rhs.age << ", " << rhs.name << "; ";
}

BOOST_AUTO_TEST_CASE(test_person_heap_custom_comparator_positive)
{
  MaxHeap< Person, PersonComparator > heap;

  heap.push(Person("Alex"));
  BOOST_CHECK(!heap.empty());
  BOOST_CHECK_EQUAL(heap.top(), Person("Alex"));
  BOOST_CHECK_EQUAL(heap.size(), 1);

  heap.push(Person("Ivan"));
  heap.push(Person("Roman"));
  heap.push(Person("Vasiliy"));
  heap.push(Person("ZZZZZZZZ"));
  BOOST_CHECK_EQUAL(heap.top(), Person("ZZZZZZZZ"));
  BOOST_CHECK_EQUAL(heap.size(), 5);

  heap.pop();
  BOOST_CHECK_EQUAL(heap.top(), Person("Vasiliy"));
  BOOST_CHECK_EQUAL(heap.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_person_heap_custom_inversed_comparator_positive)
{
  MaxHeap< Person, PersonInversedComparator > heap;

  heap.push(Person("Alex"));
  BOOST_CHECK(!heap.empty());
  BOOST_CHECK_EQUAL(heap.top(), Person("Alex"));
  BOOST_CHECK_EQUAL(heap.size(), 1);

  heap.push(Person("Ivan"));
  heap.push(Person("Roman"));
  heap.push(Person("Vasiliy"));
  heap.push(Person("AAAAAAA"));
  BOOST_CHECK_EQUAL(heap.top(), Person("AAAAAAA"));
  BOOST_CHECK_EQUAL(heap.size(), 5);

  heap.pop();
  BOOST_CHECK_EQUAL(heap.top(), Person("Alex"));
  BOOST_CHECK_EQUAL(heap.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_swap_heaps_positive)
{
  MaxHeap< int, std::less< int > > heap_A;
  MaxHeap< int, std::less< int > > heap_B;

  heap_A.push(1);
  heap_A.push(2);
  heap_A.push(3);

  heap_A.swap(heap_B);

  BOOST_CHECK(heap_A.empty());
  BOOST_CHECK_EQUAL(heap_A.size(), 0);

  BOOST_CHECK(!heap_B.empty());
  BOOST_CHECK_EQUAL(heap_B.size(), 3);
  BOOST_CHECK_EQUAL(heap_B.top(), 3);
}

class MovingObject
{
public:
  MovingObject():
      wasMoved_(false)
  {
  }

  MovingObject(const MovingObject &val):
      wasMoved_(val.wasMoved_)
  {
  }

  MovingObject(MovingObject &&val) noexcept:
      wasMoved_(val.wasMoved_)
  {
    val.wasMoved_ = true;
  }

  MovingObject &operator=(const MovingObject &rhs)
  {
    if (this != &rhs)
    {
      wasMoved_ = rhs.wasMoved_;
    }
    return *this;
  }

  MovingObject &operator=(MovingObject &&rhs) noexcept
  {
    if (this != &rhs)
    {
      wasMoved_ = rhs.wasMoved_;
      rhs.wasMoved_ = true;
    }
    return *this;
  }

  bool operator<(const MovingObject &rhs) const
  {
    return wasMoved_ < rhs.wasMoved_;
  }

  bool isMoved() const noexcept
  {
    return wasMoved_;
  }

private:
  bool wasMoved_;
};

std::ostream &operator<<(std::ostream &out, const MovingObject &rhs)
{
  return out << rhs.isMoved();
}

bool operator==(const MovingObject &lhs, const MovingObject &rhs)
{
  return lhs.isMoved() == rhs.isMoved();
}

BOOST_AUTO_TEST_CASE(test_perfect_forwarding_positive)
{
  MaxHeap< std::string, std::less< std::string > > heap_A;

  std::string str = "move semantic str";
  heap_A.push(std::move(str));

  BOOST_CHECK(heap_A.size() == 1);
  BOOST_CHECK(str.size() == 0);

  BOOST_CHECK_EQUAL(heap_A.top(), "move semantic str");
}

BOOST_AUTO_TEST_CASE(test_perfect_forwarding_Moving_Object_positive)
{
  MaxHeap< MovingObject, std::less< MovingObject > > heap_A;

  MovingObject obj;
  heap_A.push(std::move(obj));

  BOOST_CHECK(heap_A.size() == 1);
  BOOST_CHECK(obj.isMoved());
  MovingObject obj2;
  BOOST_CHECK_EQUAL(heap_A.top(), obj2);
}

BOOST_AUTO_TEST_CASE(test_heap_sort_property_stress)
{
  MaxHeap< int, std::less< int > > heap;

  std::mt19937 gen(42);
  std::uniform_int_distribution< int > dist(-1000, 1000);

  for (size_t i = 0; i < 500; ++i)
  {
    heap.push(dist(gen));
  }

  int prev_max = heap.top();
  heap.pop();

  while (!heap.empty())
  {
    int current_max = heap.top();
    BOOST_CHECK(prev_max >= current_max);
    prev_max = current_max;
    heap.pop();
  }
}

BOOST_AUTO_TEST_SUITE_END()
