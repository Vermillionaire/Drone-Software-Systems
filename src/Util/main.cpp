

#include "libfreenect.h"
#include "DataControl.h"
#include "DataProcessing.h"
#include "SpinArray.h"
#include "Log.h"
#include "WebPage.h"
#include <boost/atomic.hpp>

#include <iostream>
#include <signal.h>
#include <thread>
#include <chrono>

void signalHandler(int signal)
{
	if (signal == SIGINT
	 || signal == SIGTERM
	 || signal == SIGQUIT)
	{
		DataControl::ready = false;
	}
}



int main(int argc, char** argv) {
    Log::outln(argc, ": Number of args.");


    DataControl *co = new DataControl();

    if (argc >= 2) {
        delete co;
        return 0;
    }

    DataProcessing *cp = new DataProcessing();

    // Handle signals gracefully.
		signal(SIGINT, signalHandler);
		signal(SIGTERM, signalHandler);
		signal(SIGQUIT, signalHandler);

		//WebPage wp;
		//wp.startServer();


    std::cout << "Running\n";
		while (DataControl::ready && co->errorCheck()) {
    }

    Log::outln("All done.");
    Log::outln(DataControl::ready, "Ready value.");

    cp->joinAll();
	//	wp.stopServer();

    delete cp;
    delete co;


    return 0;
}
