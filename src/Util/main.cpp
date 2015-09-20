

#include "libfreenect.h"
#include "DataControl.h"
#include "DataProcessing.h"
#include "SpinArray.h"
#include "Log.h"
#include "WebPage.h"

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

		signal(SIGINT, signalHandler);
		signal(SIGTERM, signalHandler);
		signal(SIGQUIT, signalHandler);


    DataControl *co = new DataControl();

    if (argc >= 2) {
        delete co;
        return 0;
    }

    DataProcessing *cp = new DataProcessing();

		//WebPage wp;
		//wp.startServer();

    std::cout << "Running\n";
		while (DataControl::ready && co->errorCheck()) {
    }

    Log::outln("All done.");
    Log::outln(DataControl::ready, "Ready value.");

    cp->joinAll();
	//	wp.stopServer();
		std::cout << "Average Times:\n";
		std::cout << "Put: " << DataControl::timer << "/" << DataControl::tcount << "=" << (DataControl::timer/DataControl::tcount/1000000) << "ms" << std::endl;
		std::cout << "Get: " << DataControl::gett << "/" << DataControl::gcount << "=" << (DataControl::gett/DataControl::gcount/1000000) << "ms" << std::endl;
		std::cout << "Compute: " << DataControl::ctimer << "/" << DataControl::ccount << "=" << (DataControl::ctimer/DataControl::ccount/1000000) << "ms" << std::endl;

    delete cp;
    delete co;


//		WebPage wp;
//	  wp.startServer();

//		while (true) {};
		//seg fault
		//wp.stopServer();

    return 0;
}
