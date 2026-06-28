#include <algorithm>
#include "haffman.hpp"
#include <my_vector/top-it-vector.hpp>

void zubarev::Huffman::destroy(BSTreeNode< size_t, char >* node)
{
  if (!node) {
    return;
  }

  destroy(node->left_);
  destroy(node->right_);

  node->left_ = node->right_ = node->parent_ = nullptr;
  delete node;
}
zubarev::Huffman::Huffman():
  root_(nullptr)
{}
zubarev::Huffman::~Huffman()
{
  destroy(root_);
}
std::string zubarev::Huffman::encode(const std::string& text)
{

  destroy(root_);
  root_ = nullptr;
  codes_ = {};
  BSTree< char, size_t, Comparator< char > > freq_let = calculateLetters(text);
  root_ = treeLetters(freq_let);

  generateCodes(root_, "", codes_);

  size_t total_bits = 0;
  for (auto it = text.begin(); it != text.end(); ++it) {
    total_bits += codes_[*it].size();
  }
  std::string out_code;
  out_code.reserve(total_bits);

  for (auto it = text.begin(); it != text.end(); ++it) {
    out_code += codes_[*it];
  }
  return out_code;
}
bool zubarev::Huffman::is_leaf(const BSTreeNode< size_t, char >* n)
{
  return n && !n->left_ && !n->right_;
}
std::string zubarev::Huffman::decode(const std::string& code)
{
  BSTreeNode< size_t, char >* current = root_;
  std::string out_str = "";
  out_str.reserve(code.size() / 2);
  for (auto it = code.begin(); it != code.end(); ++it) {

    if (*it == '0') {
      current = current->left_;
    } else {
      current = current->right_;
    }
    if (!current) {
      throw std::runtime_error("Invalid huffman code");
    }
    if (is_leaf(current)) {
      out_str += current->data_.second;
      current = root_;
    }
  }
  if (current != root_) {
    throw std::runtime_error("Trailing bits do not form a complete code");
  }
  return out_str;
}
zubarev::BSTree< char, size_t, zubarev::Comparator< char > > zubarev::Huffman::calculateLetters(const std::string& text)
{
  BSTree< char, size_t, Comparator< char > > letters;

  for (auto it = text.begin(); it != text.end(); ++it) {

    letters[*it]++;
  }
  return letters;
}

zubarev::BSTreeNode< size_t, char >* zubarev::Huffman::treeLetters(BSTree< char, size_t, Comparator< char > > frequency)
{
  using HuffNode = BSTreeNode< size_t, char >;

  Vector< HuffNode* > tree;
  for (auto it = frequency.begin(); it != frequency.end(); ++it) {
    tree.pushBack(new HuffNode(it->second, it->first));
  }

  auto tree_comp = [](const HuffNode* a, const HuffNode* b) { return a->data_.first > b->data_.first; };

  if (tree.getSize() == 1) {
    HuffNode* root = tree.back();
    HuffNode* fake_parent = new HuffNode(root->data_.first, '\0');
    fake_parent->left_ = root;
    root->parent_ = fake_parent;
    return fake_parent;
  }
  while (tree.getSize() > 1) {
    std::sort(tree.begin(), tree.end(), tree_comp);

    HuffNode* left_child = tree.back();
    tree.popBack();

    HuffNode* right_child = tree.back();
    tree.popBack();

    size_t sum_weight = left_child->data_.first + right_child->data_.first;
    HuffNode* parent_node = new HuffNode(sum_weight, '\0');
    parent_node->left_ = left_child;
    parent_node->right_ = right_child;
    left_child->parent_ = parent_node;
    right_child->parent_ = parent_node;

    tree.pushBack(parent_node);
  }
  if (tree.isEmpty())
    return nullptr;
  return tree.front();
}

void zubarev::Huffman::generateCodes(const BSTreeNode< size_t, char >* node,
                                     const std::string& code,
                                     BSTree< char, std::string, Comparator< char > >& out_dictionary)
{
  if (!node) {
    return;
  }
  if (is_leaf(node)) {
    out_dictionary[node->data_.second] = code;
  }

  generateCodes(node->left_, code + "0", out_dictionary);

  generateCodes(node->right_, code + "1", out_dictionary);
}
zubarev::Vector< uint8_t > zubarev::Huffman::compress(const std::string& text)
{
  size_t bit_count = 0;
  Vector< uint8_t > packed_bytes;
  uint8_t cur_byte = 0;

  for (auto it = text.begin(); it != text.end(); ++it) {
    if (*it != '0' && *it != '1') {
      throw std::invalid_argument("compress: input must contain only '0' and '1'");
    }
    cur_byte <<= 1;

    if (*it == '1') {
      cur_byte |= 1;
    }
    bit_count++;

    if (bit_count == 8) {
      packed_bytes.pushBack(cur_byte);
      bit_count = 0;
      cur_byte = 0;
    }
  }

  if (bit_count > 0) {
    cur_byte <<= (8 - bit_count);
    packed_bytes.pushBack(cur_byte);
  }

  return packed_bytes;
}
std::string zubarev::Huffman::decompress(const Vector< uint8_t >& compress_bytes, size_t total_bits)
{
  std::string bit_string = "";
  size_t few_bits = 0;

  for (auto it = compress_bytes.begin(); it != compress_bytes.end(); ++it) {

    uint8_t byte = *it;

    for (int i = 7; i >= 0; --i) {
      if (few_bits >= total_bits) {
        break;
      }

      if ((byte >> i) & 1) {
        bit_string += '1';
      } else {
        bit_string += '0';
      }

      few_bits++;
    }
  }
  return bit_string;
}
const zubarev::BSTree< char, std::string, zubarev::Comparator< char > >& zubarev::Huffman::getCodes() const
{
  return codes_;
}
void zubarev::Huffman::buildTreeFromDictionary(const BSTree< char, std::string, Comparator< char > >& dictionary)
{
  destroy(root_);
  root_ = nullptr;
  codes_ = dictionary;

  using HuffNode = BSTreeNode< size_t, char >;
  root_ = new HuffNode(0, '\0');

  for (auto it = dictionary.begin(); it != dictionary.end(); ++it) {
    char symbol = (*it).first;
    const std::string& code = (*it).second;

    HuffNode* current = root_;
    for (char bit : code) {
      if (bit == '0') {
        if (!current->left_) {
          current->left_ = new HuffNode(0, '\0');
          current->left_->parent_ = current;
        }
        current = current->left_;
      } else {
        if (!current->right_) {
          current->right_ = new HuffNode(0, '\0');
          current->right_->parent_ = current;
        }
        current = current->right_;
      }
    }
    current->data_.second = symbol;
  }
}
