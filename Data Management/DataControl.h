#include "libfreenect.h"

class DataControl
{
    public:
        DataControl();
        ~DataControl();
        bool errorCheck();
        static bool ready;
        static void localCallback(freenect_device *ldev, void *data, uint32_t time);
    private:
        freenect_context* ctx;
        freenect_device* dev;


};
