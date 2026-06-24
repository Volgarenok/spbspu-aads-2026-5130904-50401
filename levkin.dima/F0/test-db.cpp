#include <boost/test/unit_test.hpp>
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

  auto ch1 =
      std::make_unique< RLNode >("child1", 200.0f, 100.0f, FlexDirection::Row);
  auto ch2 =
      std::make_unique< RLNode >("child2", 400.0f, 100.0f, FlexDirection::Row);

  RLNode* child1 = ch1.get();
  RLNode* child2 = ch2.get();

  rootNode->root.addChild(std::move(ch1));
  rootNode->root.addChild(std::move(ch2));

  db.calculateLayout();

  BOOST_CHECK_EQUAL(child1->box.x, 0.0f);
  BOOST_CHECK_EQUAL(child2->box.x, 200.0f);

  BOOST_CHECK_EQUAL(child1->box.width, 200.0f);
  BOOST_CHECK_EQUAL(child2->box.width, 400.0f);
}

BOOST_AUTO_TEST_CASE(test_basic_column_layout)
{
  RootDB db;
  db.createNewLayout("test_col", "dummy_path", 500.0f, 1000.0f);
  RLRootNode* rootNode = db.getActive();

  rootNode->root.flexDirection = FlexDirection::Column;
  rootNode->root.justify = JustifyContent::FlexStart;
  rootNode->root.align = AlignItems::FlexStart;

  auto ch1 =
      std::make_unique< RLNode >("child1", 100.0f, 150.0f, FlexDirection::Row);
  auto ch2 =
      std::make_unique< RLNode >("child2", 100.0f, 300.0f, FlexDirection::Row);

  RLNode* child1 = ch1.get();
  RLNode* child2 = ch2.get();

  rootNode->root.addChild(std::move(ch1));
  rootNode->root.addChild(std::move(ch2));

  db.calculateLayout();

  BOOST_CHECK_EQUAL(child1->box.y, 0.0f);
  BOOST_CHECK_EQUAL(child2->box.y, 150.0f);

  BOOST_CHECK_EQUAL(child1->box.height, 150.0f);
  BOOST_CHECK_EQUAL(child2->box.height, 300.0f);
}

BOOST_AUTO_TEST_CASE(test_justify_content_center_and_space_between)
{
  {
    RootDB db;
    db.createNewLayout("test_space_between", "dummy_path", 1000.0f, 500.0f);
    RLRootNode* rootNode = db.getActive();

    rootNode->root.flexDirection = FlexDirection::Row;
    rootNode->root.justify = JustifyContent::SpaceBetween;

    auto ch1 = std::make_unique< RLNode >(
        "child1", 200.0f, 100.0f, FlexDirection::Row);
    auto ch2 = std::make_unique< RLNode >(
        "child2", 200.0f, 100.0f, FlexDirection::Row);

    RLNode* child1 = ch1.get();
    RLNode* child2 = ch2.get();

    rootNode->root.addChild(std::move(ch1));
    rootNode->root.addChild(std::move(ch2));

    db.calculateLayout();

    BOOST_CHECK_EQUAL(child1->box.x, 0.0f);
    BOOST_CHECK_EQUAL(child2->box.x, 800.0f);
  }

  {
    RootDB db;
    db.createNewLayout("test_center", "dummy_path", 500.0f, 600.0f);
    RLRootNode* rootNode = db.getActive();

    rootNode->root.flexDirection = FlexDirection::Column;
    rootNode->root.justify = JustifyContent::Center;

    auto ch1 = std::make_unique< RLNode >(
        "child1", 100.0f, 100.0f, FlexDirection::Column);
    auto ch2 = std::make_unique< RLNode >(
        "child2", 100.0f, 100.0f, FlexDirection::Column);

    RLNode* child1 = ch1.get();
    RLNode* child2 = ch2.get();

    rootNode->root.addChild(std::move(ch1));
    rootNode->root.addChild(std::move(ch2));

    db.calculateLayout();

    BOOST_CHECK_EQUAL(child1->box.y, 200.0f);
    BOOST_CHECK_EQUAL(child2->box.y, 300.0f);
  }
}

BOOST_AUTO_TEST_CASE(test_align_items_center)
{
  RootDB db;
  db.createNewLayout("test_align", "dummy_path", 1000.0f, 500.0f);
  RLRootNode* rootNode = db.getActive();

  rootNode->root.flexDirection = FlexDirection::Row;
  rootNode->root.align = AlignItems::Center;

  auto ch1 =
      std::make_unique< RLNode >("child1", 200.0f, 200.0f, FlexDirection::Row);

  RLNode* child1 = ch1.get();
  rootNode->root.addChild(std::move(ch1));

  db.calculateLayout();

  BOOST_CHECK_EQUAL(child1->box.y, 150.0f);
}

BOOST_AUTO_TEST_CASE(test_auto_size_calculation)
{
  RootDB db;
  db.createNewLayout("test_auto", "dummy_path", 1000.0f, 500.0f);
  RLRootNode* rootNode = db.getActive();

  rootNode->root.flexDirection = FlexDirection::Row;

  auto parentAuto = std::make_unique< RLNode >(
      "parent_auto", -1.0f, -1.0f, FlexDirection::Column);

  auto child =
      std::make_unique< RLNode >("child", 350.0f, 250.0f, FlexDirection::Row);

  RLNode* pAuto = parentAuto.get();
  RLNode* ch = child.get();

  parentAuto->addChild(std::move(child));
  rootNode->root.addChild(std::move(parentAuto));

  db.calculateLayout();
  BOOST_CHECK_EQUAL(pAuto->box.width, 350.0f);
  BOOST_CHECK_EQUAL(pAuto->box.height, 250.0f);

  BOOST_CHECK_EQUAL(ch->box.width, 350.0f);
  BOOST_CHECK_EQUAL(ch->box.height, 250.0f);
}

BOOST_AUTO_TEST_CASE(test_layout_with_margins_and_paddings)
{
  RootDB db;
  db.createNewLayout("test_offsets", "dummy_path", 1000.0f, 500.0f);
  RLRootNode* rootNode = db.getActive();

  rootNode->root.flexDirection = FlexDirection::Row;
  rootNode->root.justify = JustifyContent::FlexStart;
  rootNode->root.align = AlignItems::Center;

  rootNode->root.padding.left = 50.0f;
  rootNode->root.padding.right = 0.0f;
  rootNode->root.padding.top = 30.0f;
  rootNode->root.padding.bottom = 0.0f;

  auto ch1 =
      std::make_unique< RLNode >("child1", 200.0f, 100.0f, FlexDirection::Row);
  ch1->margin.left = 20.0f;
  ch1->margin.right = 10.0f;
  ch1->margin.top = 15.0f;
  ch1->margin.bottom = 15.0f;

  auto ch2 =
      std::make_unique< RLNode >("child2", 300.0f, 100.0f, FlexDirection::Row);
  ch2->margin.left = 25.0f;
  ch2->margin.right = 0.0f;
  ch2->margin.top = 0.0f;
  ch2->margin.bottom = 0.0f;

  RLNode* child1 = ch1.get();
  RLNode* child2 = ch2.get();

  rootNode->root.addChild(std::move(ch1));
  rootNode->root.addChild(std::move(ch2));

  db.calculateLayout();

  BOOST_CHECK_EQUAL(child1->box.x, 70.0f);
  BOOST_CHECK_EQUAL(child1->box.y, 215.0f);
  BOOST_CHECK_EQUAL(child2->box.x, 305.0f);
  BOOST_CHECK_EQUAL(child2->box.y, 215.0f);
  BOOST_CHECK_EQUAL(child1->box.width, 200.0f);
  BOOST_CHECK_EQUAL(child2->box.width, 300.0f);

  // rl::saveVisualTree(db.getActive(), "layout_offsets_debug.html");
}

BOOST_AUTO_TEST_CASE(test_deeply_nested_complex_layout)
{
  RootDB db;
  db.createNewLayout("complex_app", "dummy_path", 1920.0f, 1080.0f);
  RLRootNode* rootNode = db.getActive();

  rootNode->root.flexDirection = FlexDirection::Column;
  rootNode->root.justify = JustifyContent::FlexStart;
  rootNode->root.align = AlignItems::FlexStart;
  rootNode->root.padding = {10.0f, 10.0f, 10.0f, 10.0f};

  auto headerNode =
      std::make_unique< RLNode >("header", 1900.0f, 80.0f, FlexDirection::Row);
  headerNode->margin.bottom = 20.0f;
  RLNode* headerPtr = headerNode.get();
  rootNode->root.addChild(std::move(headerNode));

  auto mainContainer = std::make_unique< RLNode >(
      "main_container", 1900.0f, -1.0f, FlexDirection::Row);
  mainContainer->padding = {15.0f, 15.0f, 15.0f, 15.0f};
  RLNode* mainContainerPtr = mainContainer.get();

  auto sidebar = std::make_unique< RLNode >(
      "sidebar", 300.0f, 600.0f, FlexDirection::Column);
  sidebar->margin.right = 30.0f;
  RLNode* sidebarPtr = sidebar.get();
  mainContainerPtr->addChild(std::move(sidebar));

  auto contentArea = std::make_unique< RLNode >(
      "content_area", -1.0f, -1.0f, FlexDirection::Column);
  contentArea->justify = JustifyContent::SpaceBetween;
  RLNode* contentAreaPtr = contentArea.get();

  auto cardRow = std::make_unique< RLNode >(
      "card_row", 1200.0f, 250.0f, FlexDirection::Row);
  cardRow->justify = JustifyContent::Center;
  cardRow->align = AlignItems::Center;
  RLNode* cardRowPtr = cardRow.get();
  contentAreaPtr->addChild(std::move(cardRow));

  auto card1 =
      std::make_unique< RLNode >("card_1", 400.0f, 180.0f, FlexDirection::Row);
  card1->margin.right = 20.0f;
  auto card2 =
      std::make_unique< RLNode >("card_2", 400.0f, 180.0f, FlexDirection::Row);

  RLNode* card1Ptr = card1.get();
  RLNode* card2Ptr = card2.get();
  cardRowPtr->addChild(std::move(card1));
  cardRowPtr->addChild(std::move(card2));

  mainContainerPtr->addChild(std::move(contentArea));
  rootNode->root.addChild(std::move(mainContainer));

  rootNode->mapOfNodes.add("header", headerPtr);
  rootNode->mapOfNodes.add("main_container", mainContainerPtr);
  rootNode->mapOfNodes.add("sidebar", sidebarPtr);
  rootNode->mapOfNodes.add("content_area", contentAreaPtr);
  rootNode->mapOfNodes.add("card_row", cardRowPtr);
  rootNode->mapOfNodes.add("card_1", card1Ptr);
  rootNode->mapOfNodes.add("card_2", card2Ptr);

  db.calculateLayout();

  BOOST_CHECK_EQUAL(headerPtr->box.x, 10.0f);
  BOOST_CHECK_EQUAL(headerPtr->box.y, 10.0f);
  BOOST_CHECK_EQUAL(mainContainerPtr->box.x, 10.0f);
  BOOST_CHECK_EQUAL(mainContainerPtr->box.y, 110.0f);
  BOOST_CHECK_EQUAL(sidebarPtr->box.x, 25.0f);
  BOOST_CHECK_EQUAL(sidebarPtr->box.y, 125.0f);
  BOOST_CHECK_EQUAL(contentAreaPtr->box.x, 355.0f);
  BOOST_CHECK_EQUAL(contentAreaPtr->box.y, 125.0f);
  BOOST_CHECK_EQUAL(cardRowPtr->box.x, 355.0f);
  BOOST_CHECK_EQUAL(cardRowPtr->box.y, 125.0f);
  BOOST_CHECK_EQUAL(card1Ptr->box.x, 545.0f);
  BOOST_CHECK_EQUAL(card1Ptr->box.y, 160.0f);
  BOOST_CHECK_EQUAL(card2Ptr->box.x, 965.0f);
  BOOST_CHECK_EQUAL(card2Ptr->box.y, 160.0f);
  BOOST_CHECK_EQUAL(cardRowPtr->box.width, 1200.0f);
  BOOST_CHECK_EQUAL(contentAreaPtr->box.width, 1200.0f);
  rl::saveVisualTree(db.getActive(), "complex_nesting_debug.html");
  rl::saveVisualTreeToPng(db.getActive(), "complex_nesting_debug.png");
}
