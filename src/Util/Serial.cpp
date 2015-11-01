
#include <iostream>
#include <signal.h>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>
#include "boost/bind.hpp"

#include "Serial.h"

using namespace std;

Serial::Serial() {
};

Serial::~Serial() {
};

/*
void Serial::write(char buffer[], int size) {
  boost::asio::write(*sp,boost::asio::buffer(buffer,size));
};


void Serial::write_1() {
  std::cout << "Writing" << std::endl;
  string s = "1";
  boost::asio::write(*sp,boost::asio::buffer(s.c_str(),s.size()));
};
*/

void Serial::start_read() {
  reading = true;
  read = new std::thread(&Serial::async_read, this);
};

void Serial::stop_read() {
  reading = false;
};

void Serial::async_read() {
  unsigned char angle_buff[50];

  std::cout << "Starting read loop." << std::endl;
  while (reading) {
    std::cout << "In read loop." << std::endl;
    sp->read_some(boost::asio::buffer(angle_buff, 50));
    string s(reinterpret_cast<char*>(angle_buff));
    std::cout << "Got: " << s << std::endl;
    string x = s.substr(0, s.find(" "));
    string y = s.substr(1, s.find(" "));
    string z = s.substr(2, s.find(" "));
    std::cout << "X: " << x << std::endl;
    std::cout << "Y: " << y << std::endl;
    std::cout << "Z: " << x << std::endl;
    avail = true;
  }
};

float* Serial::get_vals() {
  float* vals = new float[3];
  vals[0] = x;
  vals[1] = y;
  vals[2] = z;
  avail = false;
  return vals;
};

void Serial::close() {
  std::cout << "Ending serial stream" << std::cout;

  reading = false;

  if (read != nullptr)
    read->join();

  sp->cancel();
  sp->close();

/*
  while(sp->is_open()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  delete sp;
*/
};

void Serial::open(std::string port) {
  boost::asio::io_service ios;
  sp = new boost::asio::serial_port(ios);

  sp->open(port);
  sp->set_option(boost::asio::serial_port::baud_rate(115200));
  sp->set_option( boost::asio::serial_port_base::flow_control( boost::asio::serial_port_base::flow_control::none ) );
  sp->set_option( boost::asio::serial_port_base::parity( boost::asio::serial_port_base::parity::none ) );
  sp->set_option( boost::asio::serial_port_base::stop_bits( boost::asio::serial_port_base::stop_bits::one ) );
  sp->set_option( boost::asio::serial_port_base::character_size( 8 ) );
};

bool Serial::available() {
  return avail;
};
