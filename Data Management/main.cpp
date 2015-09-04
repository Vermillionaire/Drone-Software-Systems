#include <DataControl.h>
#include <iostream>
#include <signal.h>


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
    DataControl* to = new DataControl();
// Handle signals gracefully.
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGQUIT, signalHandler);


    std::cout << "Running\n";
    while (DataControl::ready && to->errorCheck()) {    }

    std::cout << "All done\n";
    delete to;
}


