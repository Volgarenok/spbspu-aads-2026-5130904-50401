#include <iostream>
#include "network.hpp"
int main()
{

  malashenko::Network net;

  net.createUser("user1");
  net.createUser("user2");
  net.createUser("user3");
  net.sendMsg("user1", "user2", "hello, how are u");
  net.sendMsg("user1", "user2", "lo, how ar");
  net.sendMsg("user1", "user2", ", how ab");
  net.sendMsg("user1", "user2", ", how ansdfsdf");
  net.sendMsg("user1", "user2", "a");
  net.sendMsg("user2", "user1", "hi");
  net.sendMsg("user2", "user1", ", how a");
  net.sendMsg("user2", "user1", "??");


  net.findMsg(std::cout, ", how a");



}
