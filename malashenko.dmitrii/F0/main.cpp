#include <iostream>
#include "network.hpp"
int main()
{

  malashenko::Network net;

  net.createUser("user1");
  net.createUser("user2");
  net.createUser("user3");
  net.createUser("user4");
  net.createUser("user5");
  net.createUser("user6");
  net.createUser("user7");
  net.createUser("user8");
  net.createUser("user9");
  net.createUser("user10");
  net.createUser("user11");
  net.createUser("user12");
  net.createUser("user13");

  net.sendMsg("user1", "user2", "a");
  net.sendMsg("user1", "user3", "a");

  net.sendMsg("user2", "user4", "a");
  net.sendMsg("user2", "user5", "a");

  net.sendMsg("user3", "user5", "a");
  net.sendMsg("user3", "user6", "a");

  net.sendMsg("user4", "user7", "a");

  net.sendMsg("user5", "user7", "a");
  net.sendMsg("user5", "user8", "a");

  net.sendMsg("user6", "user8", "a");

  net.sendMsg("user7", "user9", "a");

  net.sendMsg("user8", "user10", "a");

  net.sendMsg("user9", "user11", "a");

  net.sendMsg("user10", "user11", "a");

  net.sendMsg("user11", "user12", "a");




  net.pathBetweanUsers(std::cout, "user1", "user12");



}
