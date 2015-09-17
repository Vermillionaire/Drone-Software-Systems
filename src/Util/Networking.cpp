#include "Networking.h"
#include <boost/asio.hpp>


using namespace boost::asio;
using namespace boost::asio::ip;

Networking::Networking(int port) {
  io_service io_service;
  tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

  socket = new tcp::socket(io_service);
  acceptor.listen();
  acceptor.accept(*socket);

}

void Networking::write(Point p) {
  boost::system::error_code ignored_error;
  std::vector<boost::asio::const_buffer> buffers;
  buffers.push_back( boost::asio::buffer(&p.z, sizeof(p.z) ) );
  buffers.push_back( boost::asio::buffer(&p.x, sizeof(p.x) ) );
  buffers.push_back( boost::asio::buffer(&p.y, sizeof(p.y) ) );
  boost::asio::write(socket, buffers);
}

/*
void Networking::accept_handler(const boost::system::error_code &ec)
{
  if (!ec)
  {
    boost::system::error_code ignored_error;
    async_write(*socket, buffer(std::strimg("s")), ignored_error);
  }
}
/*
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
*/
