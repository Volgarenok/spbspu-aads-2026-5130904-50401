#include <boost/test/unit_test.hpp>
#include <string>
#include "myrbt.hpp"

using rbt_is = goltsov::RBTree< int, std::string, std::less< int > >;
using rbt_is_it = goltsov::RBTIterator< int, std::string >;
using rbt_is_cit = goltsov::RBTConstIterator< int, std::string >;

BOOST_AUTO_TEST_SUITE(RBTree_suite)

BOOST_AUTO_TEST_CASE(constructor_empty_test)
{
  rbt_is tree;
  BOOST_CHECK(tree.size() == 0);
  BOOST_CHECK(tree.begin() == tree.end());
  BOOST_CHECK(tree.height() == 0);
  BOOST_CHECK(tree.blackHeight() == 0);
}

BOOST_AUTO_TEST_CASE(insert_and_size_test)
{
  rbt_is tree;
  auto res1 = tree.insert({10, "ten"});
  BOOST_CHECK(res1.second == true);
  BOOST_CHECK((*res1.first).second == "ten");
  BOOST_CHECK(tree.size() == 1);

  auto res2 = tree.insert({20, "twenty"});
  BOOST_CHECK(res2.second == true);
  BOOST_CHECK(tree.size() == 2);

  auto res3 = tree.insert({5, "five"});
  BOOST_CHECK(res3.second == true);
  BOOST_CHECK(tree.size() == 3);

  auto res4 = tree.insert({10, "duplicate"});
  BOOST_CHECK(res4.second == false);
  BOOST_CHECK(res4.first->second == "ten");
  BOOST_CHECK(tree.size() == 3);
}

BOOST_AUTO_TEST_CASE(find_and_at_test)
{
  rbt_is tree;
  tree.insert({10, "ten"});
  tree.insert({20, "twenty"});
  tree.insert({5, "five"});

  rbt_is_it it = tree.find(20);
  BOOST_CHECK(it != tree.end());
  BOOST_CHECK(it->second == "twenty");

  const rbt_is const_tree = tree;
  rbt_is_cit const_it = const_tree.find(5);
  BOOST_CHECK(const_it != const_tree.end());
  BOOST_CHECK(const_it->second == "five");

  BOOST_CHECK(tree.find(100) == tree.end());
  BOOST_CHECK(const_tree.find(100) == const_tree.end());

  BOOST_CHECK(tree.at(10) == "ten");
  BOOST_CHECK_THROW(tree.at(100), std::logic_error);
}

BOOST_AUTO_TEST_CASE(get_last_test)
{
  rbt_is tree;
  tree.insert({10, "ten"});
  tree.insert({20, "twenty"});
  tree.insert({5, "five"});

  rbt_is_it it = tree.getLast();
  BOOST_CHECK(it->first == 20);

  const rbt_is const_tree = tree;
  rbt_is_cit const_it = const_tree.getLast();
  BOOST_CHECK(const_it->first == 20);
}

BOOST_AUTO_TEST_CASE(iterator_traversal_test)
{
  rbt_is tree;
  tree.insert({10, "ten"});
  tree.insert({20, "twenty"});
  tree.insert({5, "five"});

  rbt_is_it it = tree.begin();
  BOOST_CHECK(it != tree.end());
  BOOST_CHECK(it->first == 5);
  ++it;
  BOOST_CHECK(it != tree.end());
  BOOST_CHECK(it->first == 10);
  ++it;
  BOOST_CHECK(it != tree.end());
  BOOST_CHECK(it->first == 20);
  ++it;
  BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(erase_test)
{
  rbt_is tree;
  tree.insert({10, "ten"});
  tree.insert({20, "twenty"});

  size_t erased = tree.erase(10);
  BOOST_CHECK(erased == 1);
  BOOST_CHECK(tree.size() == 1);
  BOOST_CHECK(tree.find(10) == tree.end());

  size_t erased_none = tree.erase(100);
  BOOST_CHECK(erased_none == 0);
  BOOST_CHECK(tree.size() == 1);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  rbt_is tree;
  tree.insert({10, "ten"});
  tree.insert({20, "twenty"});
  BOOST_CHECK(tree.size() == 2);

  tree.clear();
  BOOST_CHECK(tree.size() == 0);
  BOOST_CHECK(tree.begin() == tree.end());
  BOOST_CHECK(tree.height() == 0);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  rbt_is tree1;
  tree1.insert({10, "ten"});

  rbt_is tree2;
  tree2.insert({20, "twenty"});
  tree2.insert({30, "thirty"});

  tree1.swap(tree2);
  BOOST_CHECK(tree1.size() == 2);
  BOOST_CHECK(tree2.size() == 1);
  BOOST_CHECK(tree1.begin()->first == 20);
  BOOST_CHECK(tree2.begin()->first == 10);
}

BOOST_AUTO_TEST_CASE(copy_move_test)
{
  rbt_is tree1;
  tree1.insert({10, "ten"});
  tree1.insert({20, "twenty"});

  rbt_is tree2(tree1);
  BOOST_CHECK(tree2.size() == 2);
  BOOST_CHECK(tree2.find(10) != tree2.end());

  rbt_is tree3;
  tree3 = tree1;
  BOOST_CHECK(tree3.size() == 2);

  rbt_is tree4(std::move(tree2));
  BOOST_CHECK(tree4.size() == 2);

  rbt_is tree5;
  tree5 = std::move(tree3);
  BOOST_CHECK(tree5.size() == 2);
  BOOST_CHECK(tree3.size() == 0);
}

BOOST_AUTO_TEST_CASE(height_properties_test)
{
  rbt_is tree;
  tree.insert({10, "10"});
  BOOST_CHECK(tree.height() == 1);
  BOOST_CHECK(tree.blackHeight() == 1);

  tree.insert({5, "5"});
  tree.insert({20, "20"});
  BOOST_CHECK(tree.height() >= 2);
  BOOST_CHECK(tree.blackHeight() >= 1);

  rbt_is_it it = tree.begin();
  BOOST_CHECK(tree.height(it) == 1);
  BOOST_CHECK(tree.blackHeight(it) <= 2);
}

BOOST_AUTO_TEST_CASE(balancing_and_rbt_properties_test)
{
  rbt_is tree;
  const size_t keys_size = 10;
  int* keys = new int[keys_size]{10, 20, 30, 15, 25, 5, 1, 7, 12, 18};

  for (size_t i = 0; i < keys_size; ++i)
  {
    tree.insert({keys[i], "val"});
  }

  BOOST_CHECK(tree.height() <= 6);

  size_t h = tree.height();
  size_t bh = tree.blackHeight();
  BOOST_CHECK(bh >= (h / 2));
  BOOST_CHECK(bh <= h);

  const size_t erase_size = 3;
  int* keys_to_erase = new int[erase_size]{15, 30, 5};
  for (size_t i = 0; i < erase_size; ++i)
  {
    BOOST_CHECK(tree.erase(keys_to_erase[i]) == 1);
      if (tree.size() > 0)
    {
      BOOST_CHECK(tree.height() > 0);
      BOOST_CHECK(tree.blackHeight() > 0);
    }
  }

  int last_key = -1;
  size_t traversed_count = 0;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    if (traversed_count > 0)
    {
      BOOST_CHECK(it->first > last_key);
    }
    last_key = it->first;
    ++traversed_count;
  }
  BOOST_CHECK(traversed_count == (keys_size - erase_size));

  delete[] keys;
  delete[] keys_to_erase;
}

BOOST_AUTO_TEST_SUITE_END()
