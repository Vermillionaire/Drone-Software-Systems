#include <DataControl.h>
#include <iostream>
#include <signal.h>
#include <fstream>
#include <iostream>

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


    //File ouput of data colected by the camera
    //Should be rmemoved later
    ///////////////////////////////////////////
    ofstream outf("kinect.dat");

    if (!outf)
    {
        cerr << "Could not create file!" << endl;
        exit(1);
    }

    for (unsigned int i=0; i<DataControl::outputData.size(); i++)
        outf << DataControl::outputData[i] << endl;

    outf.close();
    ////////////////////////////////////////////

    delete to;
    exit(0);
}


