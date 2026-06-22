#define BOOST_TEST_MODULE LayoutEngineTests
#include <boost/test/included/unit_test.hpp>
#include <memory>
#include "db.hpp"
#include "node.hpp"

using namespace rl;

BOOST_AUTO_TEST_CASE(test_basic_row_layout)
{
  RootDB db;
  db.createNewLayout("test", "dummy_path", 1000.0f, 500.0f);
  RLRootNode* rootNode = db.getActive();

  rootNode->root.flexDirection = FlexDirection::Row;
  rootNode->root.justify = JustifyContent::FlexStart;
  rootNode->root.align = AlignItems::FlexStart;

  auto ch1 = std::make_unique<RLNode>("child1", 200.0f, 100.0f, FlexDirection::Row);
  auto ch2 = std::make_unique<RLNode>("child2", 400.0f, 100.0f, FlexDirection::Row);

  RLNode* child1 = ch1.get();
  RLNode* child2 = ch2.get();

  rootNode->root.addChild(std::move(ch1));
  rootNode->root.addChild(std::move(ch2));


  rootNode->calculateLayout();

  BOOST_CHECK_EQUAL(child1->box.x, 0.0f);
  BOOST_CHECK_EQUAL(child2->box.x, 200.0f); 
  
  BOOST_CHECK_EQUAL(child1->box.width, 200.0f);
  BOOST_CHECK_EQUAL(child2->box.width, 400.0f);
}