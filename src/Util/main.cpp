
#include "libfreenect.h"
#include "DataControl.h"
#include "DataProcessing.h"
#include "Log.h"

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
        //Log::outln(DataControl::ready, "Ready value.");

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


    std::cout << "Running\n";
    //int counter = 0;
    //int sleep_time = 10;
    
    
    //double * x = new double;
    //double * y = new double;
    //double * z = new double;
    freenect_raw_tilt_state *state;

/*
    int size = 75;
    double ax[size];
    double ay[size];
    double az[size];
    

    int pos = 0;
    
*/
    while (DataControl::ready && co->errorCheck()) {
        //std::cout << std::endl;
        
        //counter += sleep_time;
        //std::this_thread::sleep_for (std::chrono::milliseconds(sleep_time));
        //co->buff.printSize();
        //std::cout << "\r";
        //DataControl::buff.printSize();

        //freenect_get_mks_accel(state, x, y, z);

        /*
        freenect_update_tilt_state(to->dev);
        state = freenect_get_tilt_state(to->dev);
        freenect_get_mks_accel(state, x, y, z);

        ax[pos] = *x;
        ay[pos] = *y;
        az[pos] = *z;

        pos++;
        if (pos >= size)
            pos = 0;

        double xx = 0;
        double yy = 0;
        double zz = 0
        ;
        for (int i=0; i<size; i++) {
            xx += ax[i];
            yy += ay[i];
            zz += az[i];
        }

        std::cout << xx/size << "  " << yy/size << "  " << zz/size << "\n";

        */
        //std::cout << std::endl;
    }

    Log::outln("All done.");
    Log::outln(DataControl::ready, "Ready value.");

    cp->joinAll();

    delete cp;
    delete co;

    return 0;


}



