#include "network.hpp"
#include <queue/queue.hpp>
#include "cuckoo_hash_table.hpp"

void malashenko::Network::createUser(const std::string& username)
{
  if (users_.contains(username))
  {
    throw std::invalid_argument("This username is already taken. Please use another");
  }
  detail::User newUser{username, Vector< size_t >{}, Vector< size_t >{}};
  users_.insert(username, newUser);
  graph_.insert(username, Vector< std::string >{});
}


void malashenko::Network::removeUser(const std::string& username)
{
  if (!users_.contains(username))
  {
    std::string errorMsg = "There's no user with username: " + username;
    throw std::invalid_argument(errorMsg);
  }
  for (msgIter_t b = messages_.begin(); b != messages_.end(); ++b)
  {
    if (b->value.from == username || b->value.to == username)
    {
      messages_.erase(b->key);
    }
  }

  for (chatIter_t b = chats_.begin(); b != chats_.end(); ++b)
  {
    if (b->key.first == username || b->key.second == username)
    {
      chats_.erase(b->key);
    }
  }

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

  users_.erase(username);
}

void malashenko::Network::showAllUsers(std::ostream& out) const
{
  if (users_.empty())
  {
    out << "[THERE'S NO USERS]\n";
    return;
  }

  out << "[LIST OF ALL USERS]\n";
  size_t counter = 1;
  for (userCIter_t b = users_.begin(); b != users_.end(); ++b, ++counter)
  {
    out << counter << ". " <<  b->key << "\n";
  }
}

void malashenko::Network::sendMsg(const std::string& from, const std::string& to, const std::string& text)
{
  if (text.size() == 0)
  {
    throw std::invalid_argument("Your message is empty");
  }

  if (!users_.contains(from))
  {
    std::string errorMsg = "There's no user with username: " + from;
    throw std::invalid_argument(errorMsg);
  }

  if (!users_.contains(to))
  {
    std::string errorMsg = "There's no user with username: " + to;
    throw std::invalid_argument(errorMsg);
  }

  namespace chr = std::chrono;
  chr::time_point< chr::_V2::system_clock, chr::_V2::system_clock::duration > now = chr::system_clock::now();

  std::time_t now_c = chr::system_clock::to_time_t(now);

  detail::Message newMsg{nextMessageId_, now_c, from, to, text};

  if (chats_.contains({from, to}))
  {
    chats_.at({from, to}).pushBack(nextMessageId_);
  }
  else if (chats_.contains({to, from}))
  {
    chats_.at({to, from}).pushBack(nextMessageId_);
  }
  else
  {
    chats_.insert({from, to}, Vector< size_t >{});
    chats_.at({from, to}).pushBack(nextMessageId_);
  }

  messages_.insert(nextMessageId_, newMsg);
  users_.at(to).inbox.pushBack(nextMessageId_);
  users_.at(from).outbox.pushBack(nextMessageId_);

  if (!graph_.at(from).contains(to))
  {
    graph_.at(from).pushBack(to);
  }

  if (!graph_.at(to).contains(from))
  {
    graph_.at(to).pushBack(from);
  }

  ++nextMessageId_;
}

void malashenko::Network::removeMsg(const size_t& messageId)
{
  if (!messages_.contains(messageId))
  {
    throw std::invalid_argument("There's no message with that id");
  }
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

  messages_.erase(messageId);
}

void malashenko::Network::showMsg(std::ostream& out, const msg_t& msg) const
{
  out << "id " << msg.message_id << ". " << std::ctime(&msg.timestamp);
  out << "FROM: " << msg.from << " --> TO: " << msg.to << '\n';
  out << "\"" <<  msg.text << "\"\n";
}

void malashenko::Network::showInOutBox(std::ostream& out, const std::string& username, bool isInbox) const
{
  if (!users_.contains(username))
  {
    std::string errorMsg = "There's no user with username: " + username;
    throw std::invalid_argument(errorMsg);
  }

  detail::User user = users_.at(username);

  Vector< size_t > msgIdVec = isInbox ? user.inbox : user.outbox;

  if (msgIdVec.getSize() == 0)
  {
    out << (isInbox ? "[INBOX " : "[OUTBOX ") <<  "OF USER " << username << " IS EMPTY]\n";
    return;
  }

  out << (isInbox ? "[INBOX " : "[OUTBOX ") <<  "OF USER" << username << "]\n";

  showMsg(out, messages_.at(msgIdVec[0]));
  for (size_t i = 1; i < msgIdVec.getSize(); ++i)
  {
    out << '\n';
    showMsg(out, messages_.at(msgIdVec[i]));
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
  if (!users_.contains(user1))
  {
    std::string errorMsg = "There's no user with username: " + user1;
    throw std::invalid_argument(errorMsg);
  }
  if (!users_.contains(user2))
  {
    std::string errorMsg = "There's no user with username: " + user2;
    throw std::invalid_argument(errorMsg);
  }

  Vector< size_t > msgIdVec;

  if (chats_.contains({user1, user2}))
  {
    msgIdVec = chats_.at({user1, user2});
  }
  else if (chats_.contains({user2, user1}))
  {
    msgIdVec = chats_.at({user2, user1});
  }
  else
  {
    out << "[" << user1 << " AND " << user2 << " DIDN'T COMMUNICATE]\n";
    return;
  }

  if (msgIdVec.isEmpty())
  {
    out << "[CHAT BETWEEN " << user1 << " AND " << user2 << " IS EMPTY]\n";
    return;
  }

  out << "[CHAT BETWEEN " << user1 << " AND " << user2 << "]\n";
  showMsg(out, messages_.at(msgIdVec[0]));
  for (size_t i = 1; i < msgIdVec.getSize(); ++i)
  {
    out << '\n';
    showMsg(out, messages_.at(msgIdVec[i]));
  }
}

void malashenko::Network::findMsg(std::ostream& out, const std::string& str) const
{
  Vector< size_t > msgIdVec;
  for (msgCIter_t b = messages_.begin(); b != messages_.end(); ++b)
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
    out << "[THERE'S NO MESSAGE WITH THAT PATTERN]\n";
    return;
  }

  out << "[FOUND MESSAGES]\n";
  showMsg(out, messages_.at(msgIdVec[0]));
  for (size_t i = 1; i < msgIdVec.getSize(); ++i)
  {
    out << '\n';
    showMsg(out, messages_.at(msgIdVec[i]));
  }
}

void malashenko::Network::clearChat(const std::string& user1, const std::string& user2)
{
  if (!users_.contains(user1))
  {
    std::string errorMsg = "There's no user with username: " + user1;
    throw std::invalid_argument(errorMsg);
  }

  if (!users_.contains(user2))
  {
    std::string errorMsg = "There's no user with username: " + user2;
    throw std::invalid_argument(errorMsg);
  }

  Vector< size_t > msgIdVec;
  if (chats_.contains({user1, user2}))
  {
    msgIdVec = chats_.at({user1, user2});
    chats_.at({user1, user2}).erase(0, msgIdVec.getSize());
  }
  else if (chats_.contains({user2, user1}))
  {
    msgIdVec = chats_.at({user2, user1});
    chats_.at({user2, user1}).erase(0, msgIdVec.getSize());
  }
  else
  {
    std::string errorMsg = user1 + " and " + user2 + " did't communicate.There's no chat to be cleared";
    throw std::invalid_argument(errorMsg);
  }

  for (size_t i = 0; i < msgIdVec.getSize(); ++i)
  {
    messages_.erase(msgIdVec[i]);
  }
}

void malashenko::Network::mutualUsers(std::ostream& out, const std::string& user1, const std::string& user2) const
{
  Vector< name_t > friendsOfUser1 = graph_.at(user1);
  Vector< name_t > friendsOfUser2 = graph_.at(user2);

  size_t counter = 0;
  out << "[MUTUAL FRIENDS BETWEEN" << user1 << " AND " << user2 << "]\n";
  for (size_t i = 0; i < friendsOfUser1.getSize(); ++i)
  {
    for (size_t j = 0; j < friendsOfUser2.getSize(); ++j)
    {
      if (friendsOfUser1[i] == friendsOfUser2[j])
      {
        out << counter << ". " << friendsOfUser1[i] << '\n';
        counter++;
      }
    }
  }

  if (counter == 0)
  {
    out << "[THERE'S NO MUTUAL FRIENDS]\n";
  }
}


malashenko::Vector< std::string > malashenko::Network::bfsPath( const std::string& from, const std::string& to) const
{
  Queue< std::string > q;

  CuckooHashTable< std::string, bool, HmacHash< name_t >, SipHasher< name_t >, Equal< name_t > > visited;
  CuckooHashTable< std::string, std::string , HmacHash< name_t >, SipHasher< name_t >, Equal< name_t > > parent;

  visited[from] = true;
  q.push(from);

  while (!q.empty())
  {
    std::string cur = q.front();
    q.pop();

    if (cur == to)
    {
      break;
    }

    for (size_t i = 0; i < graph_.at(cur).getSize(); ++i)
    {
      std::string next = graph_.at(cur)[i];
      if (!visited.contains(next))
      {
        visited[next] = true;
        parent[next] = cur;
        q.push(next);
      }
    }
  }

  Vector< std::string > path;

  if (!visited.contains(to))
  {
    return path;
  }

  std::string cur = to;

  while (cur != from)
  {
    path.pushBack(cur);
    cur = parent[cur];
  }

  path.pushBack(from);
  path.reverse();
  return path;
}


void malashenko::Network::pathBetweanUsers(std::ostream& out, const std::string& from, const std::string& to) const
{
  Vector< std::string > path = bfsPath(from, to);
  if (path.isEmpty())
  {
    out << "[USERS " << from << " AND " << to << " ARE NOT CONNECTED. THERE'S NO PATH BETWEEN THEM]\n";
    return;
  }

  out << "[PATH BETWEEN " << from << " AND " << to << ']';
  out << '[' << path[0];
  for (size_t i = 1; i < path.getSize(); ++i)
  {
    out << "] --> [" << path[i];
  }
  out << ']';
  out << '\n';
}

void malashenko::Network::distanceBetweanUsers(std::ostream& out, const std::string& from, const std::string& to) const
{
  Vector< std::string > revPath = bfsPath(from, to);
  if (revPath.isEmpty())
  {
    out << "[USERS " << from << " AND " << to << " ARE NOT CONNECTED. DISTANCE CANNOT BE CALCULATED]\n";
    return;
  }
  out << revPath.getSize() - 2 << '\n';
}

void malashenko::Network::removeInactive()
{
  for (userIter_t b = users_.begin(); b != users_.end(); ++b)
  {
    if (b->value.outbox.getSize() < 2)
    {
      removeUser(b->key);
    }
  }
}

void malashenko::Network::recomendUsers(std::ostream& out, const std::string& user1) const
{
  size_t counter = 1;
  for (userCIter_t b = users_.begin(); b != users_.end(); ++b)
  {
    if (user1 != b->key &&  (!chats_.contains({user1, b->key}) || !chats_.contains({user1, b->key})))
    {
      if (bfsPath(user1, b->key).getSize() - 2 <= 2)
      {
        out << counter++ << ". " << b->key << '\n';
      }
    }
  }
}

void malashenko::Network::saveToFile(std::ostream& out) const
{
  out << users_.size() << '\n';

  for (userCIter_t it = users_.begin(); it != users_.end(); ++it)
  {
    out << it->key << '\n';

    out << it->value.inbox.getSize();
    for (size_t i = 0; i < it->value.inbox.getSize(); ++i)
    {
      out << ' ' << it->value.inbox[i];
    }
    out << '\n';

    out << it->value.outbox.getSize();
    for (size_t i = 0; i < it->value.outbox.getSize(); ++i)
    {
      out << ' ' << it->value.outbox[i];
    }
    out << '\n';
  }

  out << messages_.size() << '\n';

  for (msgCIter_t it = messages_.begin(); it != messages_.end(); ++it)
  {
    const detail::Message& msg = it->value;

    out << msg.message_id << '\n';
    out << msg.timestamp << '\n';
    out << msg.from << '\n';
    out << msg.to << '\n';
    out << msg.text << '\n';
  }
}


