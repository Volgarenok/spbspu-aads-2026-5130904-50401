#ifndef NETWORK_HPP
#define NETWORK_HPP
#include <cstddef>
#include <string>
#include <vector/vector.hpp>
#include <chrono>
#include <ctime>
#include "cuckoo_hash_table.hpp"
#include "hash_funcs.hpp"
namespace malashenko
{
  namespace detail
  {
    struct Message {
      size_t message_id;
      std::string timestamp;
      std::string from;
      std::string to;
      std::string text;
    };
    struct User {
      std::string username;
      Vector< size_t > inbox;
      Vector< size_t > outbox;
    };
  }

  class Network {
  public:
    void createUser(const std::string& username);
    void removeUser(const std::string& username);
    void showAllUsers(std::ostream& out) const;

    void sendMsg(const std::string& from, const std::string& to, const std::string& text);
    void removeMsg(const size_t& messageId);

    void showInbox(std::ostream& out, const std::string& username) const;
    void showOutbox(std::ostream& out, const std::string& username) const;
    void showChat(std::ostream& out, const std::string& user1, const std::string& user2) const;
    void findMsg(std::ostream& out, const std::string& str) const;
    void clearChat(const std::string& user1, const std::string& user2);

    void mutualUsers(std::ostream& out, const std::string& user1, const std::string& user2) const;
    void recomendUsers(std::ostream& out, const std::string& user1) const;
    void pathBetweanUsers(std::ostream& out, const std::string& from, const std::string& to) const;
    void distanceBetweanUsers(std::ostream& out, const std::string& from, const std::string& to) const;
    size_t removeInactive();

    void saveToFile(std::ostream& out) const;
    void loadFromFile(std::istream& in);

  private:
    using name_t = std::string;
    using pair_t = std::pair< name_t, name_t >;
    using user_t = detail::User;
    using msg_t = detail::Message;
    using eqName_t = std::equal_to< name_t >;
    using eqSize_t = std::equal_to< size_t >;
    using eqPair_t = std::equal_to< pair_t >;

    using userIter_t = Iterator< name_t, user_t, HmacHash< name_t >, SipHasher< name_t >, eqName_t >;
    using userCIter_t = ConstIterator< name_t, user_t, HmacHash< name_t >, SipHasher< name_t >, eqName_t >;

    using msgIter_t = Iterator< size_t, msg_t, HmacHash< size_t >, SipHasher< size_t >, eqSize_t >;
    using msgCIter_t = ConstIterator< size_t, msg_t, HmacHash< size_t >, SipHasher< size_t >, eqSize_t >;

    using chatIter_t = Iterator< pair_t, Vector< size_t >, HmacHash< pair_t >, SipHasher< pair_t >, eqPair_t >;
    using graphIter_t = Iterator< name_t, Vector< name_t >, HmacHash< name_t >, SipHasher< name_t >, eqName_t >;


    CuckooHashTable< name_t, user_t, HmacHash< name_t >, SipHasher< name_t >, eqName_t > users_;
    CuckooHashTable< size_t, msg_t, HmacHash< size_t >, SipHasher< size_t >, eqSize_t > messages_;
    CuckooHashTable< pair_t, Vector< size_t >, HmacHash< pair_t >, SipHasher< pair_t >, eqPair_t > chats_;
    CuckooHashTable< name_t, Vector< name_t >, HmacHash< name_t >, SipHasher< name_t >, eqName_t > graph_;
    size_t nextMessageId_ = 0;

    void showFullInfoMsg(std::ostream& out, const msg_t&) const;
    void showInboxMsg(std::ostream& out, const msg_t&) const;
    void showOutboxMsg(std::ostream& out, const msg_t&) const;
    void showChatMsg(std::ostream& out, const msg_t&) const;


    void showInOutBox(std::ostream& out, const std::string& username, bool isInbox) const;
    Vector< name_t > bfsPath(const name_t& from, const name_t& to) const;
    std::string getCurrentTime();
    void rebuildInboxes();
    void rebuildGraph();
    void rebuildChats();

  };

}

#endif
