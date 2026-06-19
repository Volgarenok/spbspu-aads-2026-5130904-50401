#ifndef NETWORK_HPP
#define NETWORK_HPP
#include <cstddef>
#include <string>
#include <vector/vector.hpp>
#include <chrono>
#include <ctime>
#include "cuckoo_hash_table.hpp"
#include "hash_funcs.hpp"
#include "equal_functor.hpp"
namespace malashenko
{
  namespace detail
  {
    struct Message {
      size_t message_id;
      std::time_t timestamp;
      std::string from;
      std::string to;
      std::string text;
    };
    bool operator==(const Message& rhs, const Message& lhs);

    struct User {
      std::string username;
      Vector< size_t > inbox;
      Vector< size_t > outbox;
    };
    bool operator==(const User& rhs, const User& lhs);
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
    void removeInactive();

  private:
    using name_t = std::string;
    using pair_t = std::pair< name_t, name_t >;
    using user_t = detail::User;
    using msg_t = detail::Message;

    CuckooHashTable< name_t, user_t, HmacHash< name_t >, SipHasher< name_t >, Equal< name_t > > users_;
    CuckooHashTable< size_t, msg_t, HmacHash< size_t >, SipHasher< size_t >, Equal< size_t > > messages_;
    CuckooHashTable< pair_t, Vector< size_t >, HmacHash< pair_t >, SipHasher< pair_t >, Equal< pair_t > > chats_;
    CuckooHashTable< name_t, Vector< name_t >, HmacHash< name_t >, SipHasher< name_t >, Equal< name_t > > graph_;
    size_t nextMessageId_ = 0;

    void showMsg(std::ostream& out, const msg_t&) const;
    void showInOutBox(std::ostream& out, const std::string& username, bool isInbox) const;
    Vector< name_t > bfsPath(const name_t& from, const name_t& to) const;
  };

}

#endif