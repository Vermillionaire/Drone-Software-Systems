#ifndef NETWORKING_H
#define NETWORKING_H

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio/ip/tcp.hpp>
#include "Point.h"

using namespace boost::asio::ip;

class Networking {
public:
  tcp::socket* socket;
  Networking(int port);
  ~Networking();

  void accept_handler(const boost::system::error_code &ec);
  void write(Point p);

};

#endif
