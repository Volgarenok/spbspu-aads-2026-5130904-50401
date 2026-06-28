#include <string>
#include <cstdint>

#include <my_tree/my_tree.hpp>
#include <my_tree/my_compare.hpp>
#include <my_vector/top-it-vector.hpp>
namespace zubarev
{
  class Huffman
  {
  public:
    Huffman();
    ~Huffman();
    std::string encode(const std::string& text);
    std::string decode(const std::string& code);
    Vector< uint8_t > compress(const std::string& text_bit);
    std::string decompress(const Vector< uint8_t >& compress_bytes, size_t total_bits);
    const BSTree< char, std::string, Comparator< char > >& getCodes() const;
    void buildTreeFromDictionary(const BSTree< char, std::string, Comparator< char > >& dictionary);

  private:
    void destroy(BSTreeNode< size_t, char >* node);
    bool is_leaf(const BSTreeNode< size_t, char >* n);
    BSTree< char, std::string, Comparator< char > > codes_;
    BSTreeNode< size_t, char >* root_;

    BSTree< char, size_t, Comparator< char > > calculateLetters(const std::string& text);
    BSTreeNode< size_t, char >* treeLetters(BSTree< char, size_t, Comparator< char > > frequency);

    void generateCodes(const BSTreeNode< size_t, char >* node,
                       const std::string& code,
                       BSTree< char, std::string, Comparator< char > >& out_dictionary);
  };

}
