#ifndef DATACONTROL_H
#define DATACONTROL_H

#include "libfreenect.h"
//#include "SpinArray.h"
//#include "SpinArray.h"
//#include "DataProcessing.h"
#include "DataStorage.h"
#include <vector>
#include <string>
#include <thread>
#include <string.h>


using namespace std;
//class SpinArray;
//class DataProcessing;

class DataControl
{
    public:
        DataControl();
        ~DataControl();

        bool errorCheck();
        bool isReady();
        void kill();

        static void localCallback(freenect_device *ldev, void *data, uint32_t time);

        int clean_restart();

        static long frames;
        static int flimiter;

        static int angle;
        static bool recording;

        freenect_context* ctx;
        freenect_device* dev;

      //  static SpinArray* buffer;

    private:

      //  DataProcessing* coprocessor;
        static DataStorage* store;
        void serial_thread_func();
        bool serial_thread_running;
        std::thread* serial_thread;

        //Constants constants;

        bool ready = false;
        bool error_state = false;





};

#endif
