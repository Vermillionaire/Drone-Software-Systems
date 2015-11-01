#ifndef SERIAL_H
#define SERIAL_H

#include <ctime>
#include <iostream>
#include <thread>
#include <string>

#include <boost/asio.hpp>


class Serial {
public:
  Serial();
  ~Serial();
  void write(char buffer[], int size);
  void write_1();
  void start_read();
  void stop_read();
  bool available();
  float* get_vals();
  void close();
  void open(std::string port);

private:
  void async_read();
  std::thread* read;
  boost::asio::serial_port* sp;
  float x,y,z;
  bool reading = false;
  bool avail = false;
};

#endif
