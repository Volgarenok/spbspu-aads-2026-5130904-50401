namespace donkeev
{
  template< class Key, class Value >
  struct BSTNode
  {
    std::pair< const Key, Value > data_;

    BSTNode* left;
    BSTNode* right;
    BSTNode* parent;
  };
}
