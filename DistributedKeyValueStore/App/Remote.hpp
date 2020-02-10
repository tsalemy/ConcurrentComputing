#pragma once
#include <iostream>
#include <vector>
#include "Node.hpp"
#include "Result.hpp"
#include "Message.hpp"

class Remote {
private:
  std::vector<Node> nodes;
  std::vector<int> connections;
  unsigned int hash(int key);
  unsigned int getNode(int key);
  void sendMessage(const int fd, Message message);
  RESULT awaitResponse(const int fd);
public:
  Remote(std::vector<Node> nodes);
  ~Remote();
  RESULT get(int key);
  RESULT insert(std::pair<int, int>);
};