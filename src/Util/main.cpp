

#include "libfreenect.h"
#include "DataControl.h"
#include "DataProcessing.h"
#include "SpinArray.h"
#include "Log.h"

#include <iostream>
#include <signal.h>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>
#include "boost/bind.hpp"
#include "Serial.h"


void signalHandler(int signal)
{
	if (signal == SIGINT
	 || signal == SIGTERM
	 || signal == SIGQUIT)
	{
		DataControl::ready = false;
	}
}

void serial_callback(const boost::system::error_code& error, std::size_t bytes_transferred) {
  cout << "Read " << bytes_transferred << " bytes" << endl;
}

int main(int argc, char** argv) {
    Log::outln(argc, ": Number of args.");

		signal(SIGINT, signalHandler);
		signal(SIGTERM, signalHandler);
		signal(SIGQUIT, signalHandler);


		/*
    DataControl *co = new DataControl();


    if (argc >= 2 || !DataControl::ready) {
        delete co;
        return 0;
    }


    DataProcessing *cp = new DataProcessing();
		int ret = cp->epiphanyInit();
		if (ret != 0) {
			cp->epiphanyClose();
			delete cp;
			delete co;
		}

		cp->startThread();

		//WebPage wp;
		//wp.startServer();

    std::cout << "Running\n";
		while (DataControl::ready && co->errorCheck()) {}

    Log::outln("All done.");
    //Log::outln(DataControl::ready, "Ready value.");

    //cp->joinAll();
	//	wp.stopServer();
		//std::cout << "Average Times:\n";
		//std::cout << "Put: " << DataControl::timer << "/" << DataControl::tcount << "=" << (DataControl::timer/DataControl::tcount/1000000) << "ms" << std::endl;
		//std::cout << "Get: " << DataControl::gett << "/" << DataControl::gcount << "=" << (DataControl::gett/DataControl::gcount/1000000) << "ms" << std::endl;
		//std::cout << "Compute: " << DataControl::ctimer << "/" << DataControl::ccount << "=" << (DataControl::ctimer/DataControl::ccount/1000000) << "ms" << std::endl;

		cp->join();
		cp->epiphanyClose();

		DataControl::buff.printSize();
    delete cp;
    delete co;


//		WebPage wp;
//	  wp.startServer();

//		while (true) {};
		//seg fault
		//wp.stopServer();
		*/


		string port = "/dev/ttyPS0";
		//io_service ios;
		//boost::asio::serial_port sp(ios);
		/*
		sp.open(port);
		sp.set_option(boost::asio::serial_port::baud_rate(115200));
		sp.set_option( boost::asio::serial_port_base::flow_control( boost::asio::serial_port_base::flow_control::none ) );
	  sp.set_option( boost::asio::serial_port_base::parity( boost::asio::serial_port_base::parity::none ) );
	  sp.set_option( boost::asio::serial_port_base::stop_bits( boost::asio::serial_port_base::stop_bits::one ) );
	  sp.set_option( boost::asio::serial_port_base::character_size( 8 ) );

		unsigned char angle_buff[50];
		boost::asio::async_read(sp,boost::asio::buffer(angle_buff, 20), boost::asio::transfer_at_least(1), boost::bind(serial_callback, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

		string s = "1";
		boost::asio::write(sp,boost::asio::buffer(s.c_str(),s.size()));
		sp.read_some(boost::asio::buffer(angle_buff, 50));
		std::cout << angle_buff << std::endl;
		*/

		Serial ser;
		ser.open(port);
		ser.start_read();
	//	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		//ser.write_1();
		//std::cout << ser.available() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		ser.stop_read();
		ser.close();

    return 0;
}
