#include "libfreenect.h"
#include "SpinArray.h"
#include <vector>
#include <string>



using namespace std;
class SpinArray;

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

        struct frame {
            short pixel[width][height];
        };

        static SpinArray buff;

        //Temperary variables and functions
        static vector<string> outputData;

        static long frames;
    private:

        freenect_context* ctx;
        freenect_device* dev;
};
