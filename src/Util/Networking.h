#ifndef NETWORKING_H
#define NETWORKING_H

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

class Networking {
  Networking(int port);
  ~Networking();

  tcp::socket* socket;
};

#endif
