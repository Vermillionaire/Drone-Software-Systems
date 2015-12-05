#ifndef DATACONTROL_H
#define DATACONTROL_H

#include "libfreenect.h"
#include "SpinArray.h"
#include "SpinWrapper.h"
#include "DataProcessing.h"
#include <vector>
#include <string>
#include <thread>
#include <string.h>


using namespace std;
class SpinWrapper;
class DataProcessing;

class DataControl
{
    public:
        DataControl();
        ~DataControl();

        bool errorCheck();
        bool isReady();
        void kill();

        static void localCallback(freenect_device *ldev, void *data, uint32_t time);

        //const static short width = 640;
        //const static short height = 480;
        //const static int size = width * height;
        //const static short brightness = 50;
        //static long frameLimiter;



        //static fstream file;
        //static io_service ios;
        //static serial_port sp;
        //static unsigned char angle_buff[];
        //void serial_callback(const boost::system::error_code& error, std::size_t bytes_transferred);
        int clean_restart();

        //static SpinArray buff;

        //Temporary variables and functions
        //static vector<string> outputData;
        static long frames;
        static int flimiter;
        //put
      //  static double timer;
      //  static int tcount;
        //get
      //  static double gett;
      //  static int gcount;
        //compute
      //  static double ctimer;
      //  static int ccount;

        freenect_context* ctx;
        freenect_device* dev;

    private:
        struct serial_package {
        	short header;
        	int heading;
        };

        static SpinWrapper buffer_io;
        DataProcessing* coprocessor;

        void serial_thread_func();
        bool serial_thread_running;
        static int angle;
        std::thread* serial_thread;

        //Constants constants;

        bool ready = false;
        bool error_state = false;




};

#endif
