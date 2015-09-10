

#include <DataControl.h>
#include <iostream>
#include <signal.h>
#include <fstream>

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

//class DataControl;

void signalHandler(int signal)
{
	if (signal == SIGINT
	 || signal == SIGTERM
	 || signal == SIGQUIT)
	{
		//DataControl::ready = false;
	}
}

int main(int argc, char** argv) {



    DataControl* to = new DataControl();

    // Handle signals gracefully.
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGQUIT, signalHandler);


    std::cout << "Running\n";
    int counter = 0;
    int sleep_time = 30;
    while (DataControl::ready && to->errorCheck()) {
        //std::cout << std::endl;
        counter += sleep_time;
        std::this_thread::sleep_for (std::chrono::milliseconds(sleep_time));
        DataControl::buff.printSize();

        if (counter >= 1000) {
            counter = 0;
            std::cout << std::endl << DataControl::frames << std::endl;
            DataControl::frames = 0;
        }

        //std::cout << std::endl;
       }

    std::cout << "All done\n";

    /*
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

    */
    delete to;

    exit(0);


}



