#include "network.hpp"


bool malashenko::detail::operator==(const Message& rhs, const Message& lhs)
{
  bool res = (rhs.message_id == lhs.message_id);
  res = res && (rhs.timestamp == lhs.timestamp);
  res = res && (rhs.from == lhs.from);
  res = res && (rhs.to == lhs.to);
  return res && (rhs.text == lhs.text);
}


bool malashenko::detail::operator==(const User& rhs, const User& lhs)
{
  bool res = (rhs.username == lhs.username);
  res = res && (rhs.inbox == lhs.inbox);
  return res && (rhs.outbox == lhs.outbox);
}

void malashenko::Network::createUser(const std::string& username)
{
  if (users_.has(username))
  {
    throw std::invalid_argument("This username is already taken. Please use another");
  }
  detail::User newUser{username, Vector< size_t >{}, Vector< size_t >{}};
  users_.add(username, newUser);
  graph_.add(username, Vector< std::string >{});
}


void malashenko::Network::removeUser(const std::string& username)
{
  if (!users_.has(username))
  {
    throw std::invalid_argument("There's no user with that username");
  }
  using msgIter_t = Iterator< size_t, msg_t, HmacHash< size_t >, SipHasher< size_t >, Equal< size_t > >;
  for (msgIter_t b = messages_.begin(); b != messages_.end(); ++b)
  {
    if (b->value.from == username || b->value.to == username)
    {
      messages_.drop(b->key);
    }
  }
  using chatIter_t = Iterator< pair_t,
                               Vector< size_t >,
                               HmacHash< pair_t >,
                               SipHasher< pair_t >,
                               Equal< pair_t >
                             >;

  for (chatIter_t b = chats_.begin(); b != chats_.end(); ++b)
  {
    if (b->key.first == username || b->key.second == username)
    {
      chats_.drop(b->key);
    }
  }

  using graphIter_t = Iterator< name_t, Vector< name_t >, HmacHash< name_t >, SipHasher< name_t >, Equal< name_t > >;
  for (graphIter_t b = graph_.begin(); b != graph_.end(); ++b)
  {
    for (size_t i = 0; i < b->value.getSize(); ++i)
    {
      if (b->value[i] == username)
      {
        b->value.erase(i);
      }
    }
  }

  users_.drop(username);
}

void malashenko::Network::showAllUsers(std::ostream& out) const
{
  using userIter_t = ConstIterator< name_t, user_t, HmacHash< name_t >, SipHasher< name_t >, Equal< name_t > >;
  size_t counter = 1;
  for (userIter_t b = users_.begin(); b != users_.end(); ++b, ++counter)
  {
    out << counter << ". " <<  b->key << "\n";
  }
}

void malashenko::Network::sendMsg(const std::string& from, const std::string& to, const std::string& text)
{
  if (text.size() == 0)
  {
    throw std::invalid_argument("Empty message");
  }
  if (!users_.has(from) || !users_.has(to))
  {
    throw std::invalid_argument("There's no user with that username");
  }

  std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::_V2::system_clock::duration> now = std::chrono::system_clock::now();

  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  detail::Message newMsg{nextMessageId_, now_c, from, to, text};
  if (chats_.has({from, to}))
  {
    chats_.get({from, to}).pushBack(nextMessageId_);
  }
  else if (chats_.has({to, from}))
  {
    chats_.get({to, from}).pushBack(nextMessageId_);
  }
  else
  {
    chats_.add({from, to}, Vector< size_t >{});
    chats_.get({from, to}).pushBack(nextMessageId_);
  }
  messages_.add(nextMessageId_, newMsg);
  users_.get(to).inbox.pushBack(nextMessageId_);
  users_.get(from).outbox.pushBack(nextMessageId_);
  ++nextMessageId_;
}

void malashenko::Network::removeMsg(const size_t& messageId)
{
  using chatIter_t = Iterator< pair_t,
                               Vector< size_t >,
                               HmacHash< pair_t >,
                               SipHasher< pair_t >,
                               Equal< pair_t >
                             >;

  for (chatIter_t b = chats_.begin(); b != chats_.end(); ++b)
  {
    for (size_t i = 0; i < b->value.getSize(); ++i)
    {
      if (b->value[i] == messageId)
      {
        b->value.erase(i);
      }
    }
  }

  messages_.drop(messageId);
}

void malashenko::Network::showMsg(std::ostream& out, const msg_t& msg) const
{
  out << msg.message_id << ". " << std::ctime(&msg.timestamp);
  out << "FROM: " << msg.from << " --> TO: " << msg.to << '\n';
  out << "\"" <<  msg.text << "\"\n";
}

void malashenko::Network::showInOutBox(std::ostream& out, const std::string& username, bool isInbox) const
{
  if (!users_.has(username))
  {
    throw std::invalid_argument("There's no user with that username");
  }

  detail::User user = users_.get(username);
  Vector< size_t > msgIdVec = isInbox ? user.inbox : user.outbox;

  if (msgIdVec.getSize() != 0)
  {
    showMsg(out, messages_.get(msgIdVec[0]));
    for (size_t i = 1; i < msgIdVec.getSize(); ++i)
    {
      out << '\n';
      showMsg(out, messages_.get(msgIdVec[i]));
    }
  }
}

void malashenko::Network::showInbox(std::ostream& out, const std::string& username) const
{
  showInOutBox(out, username, 1);
}


void malashenko::Network::showOutbox(std::ostream& out, const std::string& username) const
{
  showInOutBox(out, username, 0);
}

void malashenko::Network::showChat(std::ostream& out, const std::string& user1, const std::string& user2) const
{
  if (!users_.has(user1) || !users_.has(user2))
  {
    throw std::invalid_argument("There's no user with that username");
  }

  Vector< size_t > msgIdVec;
  if (chats_.has({user1, user2}))
  {
    msgIdVec = chats_.get({user1, user2});
  }
  else if (chats_.has({user2, user1}))
  {
    msgIdVec = chats_.get({user2, user1});
  }
  else
  {
    throw std::invalid_argument("This people didn't chat to each other");
  }

  if (msgIdVec.isEmpty())
  {
    out << "[EMPTY CHAT]\n";
    return;
  }

  showMsg(out, messages_.get(msgIdVec[0]));
  for (size_t i = 1; i < msgIdVec.getSize(); ++i)
  {
    out << '\n';
    showMsg(out, messages_.get(msgIdVec[i]));
  }
}

void malashenko::Network::findMsg(std::ostream& out, const std::string& str) const
{
  using msgIter_t = ConstIterator< size_t, msg_t, HmacHash< size_t >, SipHasher< size_t >, Equal< size_t > >;
  Vector< size_t > msgIdVec;
  for (msgIter_t b = messages_.begin(); b != messages_.end(); ++b)
  {
    if (b->value.text.size() < str.size())
    {
      continue;
    }

    if (b->value.text.find(str) != std::string::npos)
    {
      msgIdVec.pushBack(b->value.message_id);
    }
  }

  if (msgIdVec.isEmpty())
  {
    out << "THERE'S NO MESSAGES WITH THAT PATTERN\n";
    return;
  }
  showMsg(out, messages_.get(msgIdVec[0]));
  for (size_t i = 1; i < msgIdVec.getSize(); ++i)
  {
    out << '\n';
    showMsg(out, messages_.get(msgIdVec[i]));
  }
}

void malashenko::Network::clearChat(const std::string& user1, const std::string& user2)
{
  if (!users_.has(user1) || !users_.has(user2))
  {
    throw std::invalid_argument("There's no user with that username");
  }

  Vector< size_t > msgIdVec;
  if (chats_.has({user1, user2}))
  {
    msgIdVec = chats_.get({user1, user2});
    chats_.get({user1, user2}).erase(0, msgIdVec.getSize());
  }
  else if (chats_.has({user2, user1}))
  {
    msgIdVec = chats_.get({user2, user1});
    chats_.get({user2, user1}).erase(0, msgIdVec.getSize());
  }
  else
  {
    throw std::invalid_argument("This people didn't chat to each other");
  }

  for (size_t i = 0; i < msgIdVec.getSize(); ++i)
  {
    messages_.drop(msgIdVec[i]);
  }
}


