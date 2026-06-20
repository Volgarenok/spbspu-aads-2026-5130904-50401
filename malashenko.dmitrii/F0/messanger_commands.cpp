#include "messanger_commands.hpp"
#include <iomanip>

void malashenko::Messanger::user(std::iostream& in, std::ostream&)
{
  std::string username;
  if (!(in >> username))
  {
    throw std::invalid_argument("Input error");
  }
  net_.createUser(username);

}

void malashenko::Messanger::rmuser(std::iostream& in, std::ostream&)
{
  std::string username;
  if (!(in >> username))
  {
    throw std::invalid_argument("Input error");
  }
  net_.removeUser(username);
}

void malashenko::Messanger::ls(std::iostream&, std::ostream& out)
{
  net_.showAllUsers(out);
}

void malashenko::Messanger::send(std::iostream& in, std::ostream&)
{
  std::string usernameFrom, usernameTo, msg;
  if (!(in >> usernameFrom >> usernameTo >> std::quoted(msg)))
  {
    throw std::invalid_argument("Input error");
  }

  net_.sendMsg(usernameFrom, usernameTo, msg);
}

void malashenko::Messanger::rmmsg(std::iostream& in, std::ostream&)
{
  size_t msgId;
  if (!(in >> msgId))
  {
    throw std::invalid_argument("Input error");
  }
  net_.removeMsg(msgId);
}

void malashenko::Messanger::inbox(std::iostream& in, std::ostream& out)
{
  std::string username;
  if (!(in >> username))
  {
    throw std::invalid_argument("Input error");
  }
  net_.showInbox(out, username);
}

void malashenko::Messanger::outbox(std::iostream& in, std::ostream& out)
{
  std::string username;
  if (!(in >> username))
  {
    throw std::invalid_argument("Input error");
  }
  net_.showOutbox(out, username);
}

void malashenko::Messanger::chat(std::iostream& in, std::ostream& out)
{
  std::string username1, username2;
  if (!(in >> username1 >> username2))
  {
    throw std::invalid_argument("Input error");
  }
  net_.showChat(out, username1, username2);
}

void malashenko::Messanger::find(std::iostream& in, std::ostream& out)
{
  std::string pattern;
  if (!(in >> pattern))
  {
    throw std::invalid_argument("Input error");
  }
  net_.findMsg(out, pattern);
}

void malashenko::Messanger::clear(std::iostream& in, std::ostream&)
{
  std::string username1, username2;
  if (!(in >> username1 >> username2))
  {
    throw std::invalid_argument("Input error");
  }
  net_.clearChat(username1, username2);
}

void malashenko::Messanger::mutual(std::iostream& in, std::ostream& out)
{
  std::string username1, username2;
  if (!(in >> username1 >> username2))
  {
    throw std::invalid_argument("Input error");
  }
  net_.mutualUsers(out, username1, username2);
}


void malashenko::Messanger::recommend(std::iostream& in, std::ostream& out)
{
  std::string username;
  if (!(in >> username))
  {
    throw std::invalid_argument("Input error");
  }
  net_.recomendUsers(out, username);
}


void malashenko::Messanger::path(std::iostream& in, std::ostream& out)
{
  std::string username1, username2;
  if (!(in >> username1 >> username2))
  {
    throw std::invalid_argument("Input error");
  }
  net_.pathBetweanUsers(out, username1, username2);
}


void malashenko::Messanger::remove_inactive(std::iostream&, std::ostream&)
{
  net_.removeInactive();
}


void malashenko::Messanger::distance(std::iostream& in, std::ostream& out)
{
  std::string username1, username2;
  if (!(in >> username1 >> username2))
  {
    throw std::invalid_argument("Input error");
  }
  net_.distanceBetweanUsers(out, username1, username2);
}


