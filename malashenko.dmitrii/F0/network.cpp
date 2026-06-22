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
    std::string errorMsg = "There's no user with that username: " + username;
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

  detail::Message newMsg{nextMessageId_, getCurrentTime(), from, to, text};

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
  detail::Message msg = messages_.at(messageId);

  Vector< size_t >& inbox = users_.at(msg.to).inbox;
  for (size_t i = 0; i < inbox.getSize(); ++i)
  {
    if (inbox[i] == messageId)
    {
      inbox.erase(i);
      break;
    }
  }

  Vector< size_t >& outbox = users_.at(msg.from).outbox;
  for (size_t i = 0; i < outbox.getSize(); ++i)
  {
    if (outbox[i] == messageId)
    {
      outbox.erase(i);
      break;
    }
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

void malashenko::Network::showInboxMsg(std::ostream& out, const msg_t& msg) const
{
  out << '[' << msg.timestamp << "] " << msg.from << '\n';
  out << msg.text << "\n";
}

void malashenko::Network::showOutboxMsg(std::ostream& out, const msg_t& msg) const
{
  out << '[' << msg.timestamp << "] -> " << msg.to << '\n';
  out << msg.text << "\n";;
}

void malashenko::Network::showChatMsg(std::ostream& out, const msg_t& msg) const
{
  out << '[' << msg.timestamp << "] " << msg.from << '\n';
  out << msg.text << "\n";;
}

void malashenko::Network::showFullInfoMsg(std::ostream& out, const msg_t& msg) const
{
  out << "id " << msg.message_id << ". [" << msg.timestamp << "] ";
  out << msg.from << " -> " << msg.to << '\n';
  out << msg.text << "\n";
}

void malashenko::Network::showInOutBox(std::ostream& out, const std::string& username, bool isInbox) const
{
  if (!users_.contains(username))
  {
    std::string errorMsg = "There's no user with that username: " + username;
    throw std::invalid_argument(errorMsg);
  }

  detail::User user = users_.at(username);

  Vector< size_t > msgIdVec = isInbox ? user.inbox : user.outbox;

  if (msgIdVec.getSize() == 0)
  {
    out << (isInbox ? "[INBOX " : "[OUTBOX ") <<  "OF " << username << " IS EMPTY]\n";
    return;
  }

  out << (isInbox ? "[INBOX " : "[OUTBOX ") <<  "OF " << username << "]\n";
  out << "────────────────────────────\n";
  if (isInbox)
  {
    showInboxMsg(out, messages_.at(msgIdVec[0]));
  }
  else
  {
    showOutboxMsg(out, messages_.at(msgIdVec[0]));
  }
  out << "────────────────────────────";

  for (size_t i = 1; i < msgIdVec.getSize(); ++i)
  {
    out << '\n';
    if (isInbox)
    {
      showInboxMsg(out, messages_.at(msgIdVec[i]));
    }
    else
    {
      showOutboxMsg(out, messages_.at(msgIdVec[i]));
    }
    out << "────────────────────────────";
  }
  out << '\n';

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

  out << "========== CHAT: user1 <-> user2 ==========\n";
  showChatMsg(out, messages_.at(msgIdVec[0]));
  for (size_t i = 1; i < msgIdVec.getSize(); ++i)
  {
    out << '\n';
    showChatMsg(out, messages_.at(msgIdVec[i]));
  }
  out << "===========================================\n";
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
  out << "────────────────────────────\n";
  showFullInfoMsg(out, messages_.at(msgIdVec[0]));
  out << "────────────────────────────\n";
  for (size_t i = 1; i < msgIdVec.getSize(); ++i)
  {
    out << '\n';
    showFullInfoMsg(out, messages_.at(msgIdVec[i]));
    out << "────────────────────────────";
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

  Vector< name_t > friendsOfUser1 = graph_.at(user1);
  Vector< name_t > friendsOfUser2 = graph_.at(user2);

  size_t counter = 1;
  out << "[MUTUAL FRIENDS BETWEEN " << user1 << " AND " << user2 << "]\n";
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

  Vector< std::string > path = bfsPath(from, to);
  if (path.isEmpty())
  {
    out << "[USERS " << from << " AND " << to << " ARE NOT CONNECTED. THERE'S NO PATH BETWEEN THEM]\n";
    return;
  }

  out << "[PATH BETWEEN " << from << " AND " << to << "]\n";
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

  Vector< std::string > revPath = bfsPath(from, to);
  if (revPath.isEmpty())
  {
    out << "[USERS " << from << " AND " << to << " ARE NOT CONNECTED. DISTANCE CANNOT BE CALCULATED]\n";
    return;
  }
  out << revPath.getSize() - 2 << '\n';
}

size_t malashenko::Network::removeInactive()
{
  size_t counter = 0;
  for (userIter_t b = users_.begin(); b != users_.end(); ++b)
  {
    if (b->value.outbox.getSize() < 2)
    {
      removeUser(b->key);
      counter++;
    }
  }
  return counter;
}

void malashenko::Network::recomendUsers(std::ostream& out, const std::string& username) const
{
  if (!users_.contains(username))
  {
    std::string errorMsg = "There's no user with username: " + username;
    throw std::invalid_argument(errorMsg);
  }

  out << "[RECOMMENDED USERS FOR " << username << "]\n";
  size_t counter = 1;
  for (userCIter_t b = users_.begin(); b != users_.end(); ++b)
  {
    if (username != b->key &&  (!chats_.contains({username, b->key}) || !chats_.contains({username, b->key})))
    {
      if (bfsPath(username, b->key).getSize() - 2 <= 2)
      {
        out << counter++ << ". " << b->key << '\n';
      }
    }
  }
  if (counter == 1)
  {
    out << "[THERE'S NO USERS TO RECOMMEND]\n";
  }
}


void malashenko::Network::saveToFile(std::ostream& out) const
{
  out << users_.size() << '\n';

  for (userCIter_t it = users_.begin(); it != users_.end(); ++it)
  {
    out << it->key << '\n';
  }

  out << messages_.size() << '\n';

  for (msgCIter_t it = messages_.begin(); it != messages_.end(); ++it)
  {
    const detail::Message& msg = it->value;

    out << msg.message_id << ' ' <<  msg.timestamp << ' ';
    out << msg.from << ' ' << msg.to << ' ' << msg.text.size() << ' ' << msg.text << '\n';
  }
  out << nextMessageId_ << '\n';
}

std::string malashenko::Network::getCurrentTime()
{
  std::chrono::_V2::system_clock::time_point now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);

  std::tm* local = std::localtime(&now_c);

  std::string result;

  if (local->tm_hour < 10)
  {
    result += '0';
  }
  result += std::to_string(local->tm_hour);

  result += ':';

  if (local->tm_min < 10)
  {
    result += '0';
  }
  result += std::to_string(local->tm_min);

  return result;
}

void malashenko::Network::loadFromFile(std::istream& in)
{
  users_.clear();
  messages_.clear();
  graph_.clear();
  chats_.clear();

  size_t userCount = 0;
  in >> userCount;

  for (size_t i = 0; i < userCount; ++i)
  {
    std::string username;
    in >> username;

    detail::User user;
    user.username = username;

    users_.insert(username, user);
  }

  size_t messageCount = 0;
  in >> messageCount;

  for (size_t i = 0; i < messageCount; ++i)
  {
    detail::Message msg;

    size_t textSize = 0;

    in >> msg.message_id;
    in >> msg.timestamp;
    in >> msg.from;
    in >> msg.to;
    in >> textSize;

    in.get();

    msg.text.resize(textSize);
    in.read(&msg.text[0], textSize);

    in.ignore(1);
    messages_.insert(msg.message_id, msg);
  }

  rebuildInboxes();
  rebuildGraph();
  rebuildChats();
  in >> nextMessageId_;
}

void malashenko::Network::rebuildInboxes()
{
  for (userIter_t it = users_.begin(); it != users_.end(); ++it)
  {
    it->value.inbox.clear();
    it->value.outbox.clear();
  }

  for (msgIter_t it = messages_.begin(); it != messages_.end(); ++it)
  {
    const detail::Message& msg = it->value;

    users_.at(msg.from).outbox.pushBack(msg.message_id);
    users_.at(msg.to).inbox.pushBack(msg.message_id);
  }
}


void malashenko::Network::rebuildGraph()
{

  for (msgIter_t it = messages_.begin(); it != messages_.end(); ++it)
  {
    const detail::Message& msg = it->value;

    if (!graph_[msg.from].contains(msg.to))
    {
      graph_[msg.from].pushBack(msg.to);
    }

    if (!graph_[msg.to].contains(msg.from))
    {
      graph_[msg.to].pushBack(msg.from);
    }
  }
}

void malashenko::Network::rebuildChats()
{

  for (msgIter_t it = messages_.begin(); it != messages_.end(); ++it)
  {
    const detail::Message& msg = it->value;

    std::pair<std::string, std::string> key;

    if (msg.from < msg.to)
    {
      key = {msg.from, msg.to};
    }
    else
    {
      key = {msg.to, msg.from};
    }

    chats_[key].pushBack(msg.message_id);
  }
}
