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
  net.createUser("user8");
  net.createUser("user9");
  net.createUser("user10");
  net.createUser("user11");


  net.sendMsg("user1", "user2", "a");
  net.sendMsg("user1", "user3", "a");
  net.sendMsg("user1", "user4", "a");
  net.sendMsg("user1", "user5", "a");
  net.sendMsg("user1", "user6", "a");

  net.sendMsg("user2", "user10", "a");
  net.sendMsg("user2", "user3", "a");
  net.sendMsg("user2", "user11", "a");
  net.sendMsg("user2", "user5", "a");
  net.sendMsg("user2", "user8", "a");




  net.mutualUsers(std::cout, "user1", "user2");



}
