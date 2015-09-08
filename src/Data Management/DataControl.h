#include "libfreenect.h"
#include <vector>
#include <string>

using namespace std;

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

        //Depth point. Contains the z value, pixel locations i and j, and the time
        struct DPoint {
            short depth;
            short i;
            short j;
            long time;
        };

        struct frame {
            short pixel[width][height];
        };

        //Temperary variables and functions
        static vector<string> outputData;
    private:
        freenect_context* ctx;
        freenect_device* dev;


};
