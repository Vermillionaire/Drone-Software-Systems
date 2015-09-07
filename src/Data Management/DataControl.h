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

        //Temperary variables and functions
        static vector<string> outputData;
    private:
        freenect_context* ctx;
        freenect_device* dev;


};
