#include "messenger_commands.hpp"
#include <iomanip>
#include <fstream>
#include "cli_messages.hpp"

void malashenko::Messenger::user(std::istream& in, std::ostream& out)
{
  std::string username;
  if (!(in >> username))
  {
    throw std::invalid_argument("Input error");
  }
  net_.createUser(username);
  detail::successMsg(out, "User was successfully created");

}

void malashenko::Messenger::rmuser(std::istream& in, std::ostream& out)
{
  std::string username;
  if (!(in >> username))
  {
    throw std::invalid_argument("Input error");
  }
  net_.removeUser(username);
  detail::successMsg(out, "User was successfully removed");
}

void malashenko::Messenger::ls(std::istream&, std::ostream& out)
{
  net_.showAllUsers(out);
}

void malashenko::Messenger::send(std::istream& in, std::ostream& out)
{
  std::string usernameFrom, usernameTo, msg;
  if (!(in >> usernameFrom >> usernameTo >> std::quoted(msg)))
  {
    throw std::invalid_argument("Input error");
  }

  net_.sendMsg(usernameFrom, usernameTo, msg);
  detail::successMsg(out, "The message was sent successfully");
}

void malashenko::Messenger::rmmsg(std::istream& in, std::ostream& out)
{
  size_t msgId;
  if (!(in >> msgId))
  {
    throw std::invalid_argument("Input error");
  }
  net_.removeMsg(msgId);
  detail::successMsg(out, "The message was removed successfully");
}

void malashenko::Messenger::inbox(std::istream& in, std::ostream& out)
{
  std::string username;
  if (!(in >> username))
  {
    throw std::invalid_argument("Input error");
  }
  net_.showInbox(out, username);
}

void malashenko::Messenger::outbox(std::istream& in, std::ostream& out)
{
  std::string username;
  if (!(in >> username))
  {
    throw std::invalid_argument("Input error");
  }
  net_.showOutbox(out, username);
}

void malashenko::Messenger::chat(std::istream& in, std::ostream& out)
{
  std::string username1, username2;
  if (!(in >> username1 >> username2))
  {
    throw std::invalid_argument("Input error");
  }
  net_.showChat(out, username1, username2);
}

void malashenko::Messenger::find(std::istream& in, std::ostream& out)
{
  std::string pattern;
  if (!(in >> pattern))
  {
    throw std::invalid_argument("Input error");
  }
  net_.findMsg(out, pattern);
}

void malashenko::Messenger::clear(std::istream& in, std::ostream& out)
{
  std::string username1, username2;
  if (!(in >> username1 >> username2))
  {
    throw std::invalid_argument("Input error");
  }
  net_.clearChat(username1, username2);
  detail::successMsg(out, "Chat was successfully cleared");
}

void malashenko::Messenger::mutual(std::istream& in, std::ostream& out)
{
  std::string username1, username2;
  if (!(in >> username1 >> username2))
  {
    throw std::invalid_argument("Input error");
  }
  net_.mutualUsers(out, username1, username2);
}


void malashenko::Messenger::recommend(std::istream& in, std::ostream& out)
{
  std::string username;
  if (!(in >> username))
  {
    throw std::invalid_argument("Input error");
  }
  net_.recomendUsers(out, username);
}


void malashenko::Messenger::path(std::istream& in, std::ostream& out)
{
  std::string username1, username2;
  if (!(in >> username1 >> username2))
  {
    throw std::invalid_argument("Input error");
  }
  net_.pathBetweanUsers(out, username1, username2);
}


void malashenko::Messenger::remove_inactive(std::istream&, std::ostream& out)
{
  size_t counter = net_.removeInactive();
  std::string msg = std::to_string(counter) + " inactive users were removed";
  detail::successMsg(out, msg);
}


void malashenko::Messenger::distance(std::istream& in, std::ostream& out)
{
  std::string username1, username2;
  if (!(in >> username1 >> username2))
  {
    throw std::invalid_argument("Input error");
  }
  net_.distanceBetweanUsers(out, username1, username2);
}

void malashenko::Messenger::help(std::istream&, std::ostream& out)
{
  out << R"(

  ==================== AVAILABLE COMMANDS ====================

  USER MANAGEMENT

  user <username>
      Create a new user.

  rmuser <username>
      Remove an existing user and all related data.

  ls
      Display the list of all users.


  MESSAGES

  send <from> <to> <message>
      Send a message from one user to another.

  rmmsg <message_id>
      Delete a message by its ID.

  inbox <username>
      Show all messages received by the user.

  outbox <username>
      Show all messages sent by the user.

  find <text>
      Search messages containing the specified text.


  CHATS

  chat <user1> <user2>
      Display the conversation between two users.

  clear <user1> <user2>
      Delete all messages exchanged between two users.


  SOCIAL GRAPH

  mutual <user1> <user2>
      Show mutual interlocutors of two users.

  recommend <username>
      Show recommended interlocutors
      (friends of friends).

  path <user1> <user2>
      Find the shortest communication path
      between two users.

  distance <user1> <user2>
      Show the number of intermediaries
      between two users.


  MAINTENANCE

  remove-inactive
      Remove inactive users.

  help
      Display this help message.

  save <filename>
      Save all users, messages and chats
      to the specified file.



  load <filename>
      Load users, messages and chats
      from the specified file.

  ============================================================
  )";
}

void malashenko::Messenger::save(std::istream& in,  std::ostream& out)
{
  std::string filename;
  if (!(in >> filename))
  {
    throw std::invalid_argument("input problems");
  }
  std::ofstream file(filename);
  net_.saveToFile(file);
  detail::successMsg(out, "Data successfully saved to file");
}

void malashenko::Messenger::load(std::istream& in,  std::ostream& out)
{
  std::string filename;
  if (!(in >> filename))
  {
    throw std::invalid_argument("input problems");
  }
  std::ifstream file(filename);
  net_.loadFromFile(file);
  detail::successMsg(out, "Data successfully loaded from file");
}

