#ifndef DATACONTROL_H
#define DATACONTROL_H

#include "libfreenect.h"
#include "SpinArray.h"
#include <vector>
#include <string>
#include <thread>
#include <string.h>
//#include <fstream>
//#include <iostream>
#include <boost/asio.hpp>

using namespace std;
class SpinArray;
using namespace boost::asio;

class DataControl
{
    public:
        DataControl();
        ~DataControl();
        bool errorCheck();
        static bool ready;
        static void localCallback(freenect_device *ldev, void *data, uint32_t time);

        const static short width = 640;
        const static short height = 480;
        const static int size = width * height;
        const static short brightness = 50;
        static long frameLimiter;

        static SpinArray buff;

        //static fstream file;
        static io_service ios;
        static serial_port sp;
        static unsigned char angle_buff[];

        //static SpinArray buff;

        //Temporary variables and functions
        static vector<string> outputData;
        static long frames;
        //put
        static double timer;
        static int tcount;
        //get
        static double gett;
        static int gcount;
        //compute
        static double ctimer;
        static int ccount;

        freenect_context* ctx;
        freenect_device* dev;
    private:

        void serial_callback(const boost::system::error_code& error, std::size_t bytes_transferred);


};

#endif
