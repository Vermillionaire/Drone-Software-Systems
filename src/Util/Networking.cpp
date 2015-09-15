#include "Networking.h"

using boost::asio::ip::tcp;


Networking::Networking(int port) {
  boost::asio::io_service io_service;
  tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 9002));
  socket = new socket(io_service);
}

int main()
{
  try
  {

    for (;;)
    {
      tcp::socket socket(io_service);
      acceptor.accept(socket);
      std::string message = make_daytime_string();
      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
