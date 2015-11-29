

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

DataControl *co;

void signalHandler(int signal)
{
	if (signal == SIGINT
	 || signal == SIGTERM
	 || signal == SIGQUIT)
	{
		if (co != nullptr)
		co->kill();
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

    co = new DataControl();

    if (argc >= 2 || !co->isReady()) {
        delete co;
        return 0;
    }

    Log::outln("Ready!");

		int error_counter = 0;
		while (co->isReady()) {
			if (!co->errorCheck() && co->isReady()) {
				Log::outln(++error_counter, "Kinect device encontered an error, attempting to reconnect...");
				//std::cout << "Kinect device encontered an error, attempting to reconnect... (" << ++error_counter << ")" << std::endl;
				co->clean_restart();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

				if (error_counter > 15)
					co->kill();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}

    Log::outln("All done.");

    delete co;

    return 0;
}
