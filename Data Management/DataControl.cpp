#include "DataControl.h"
#include <iostream>
#include <thread>
bool DataControl::ready = false;

void DataControl::localCallback(freenect_device *ldev, void *data, uint32_t time) {
    //std::cout << "Getting stuff\n";
    //DataControl::ready = false;

    short* test = static_cast<short*>(data);

    for (int i=0; i<480; i+=5) {
        for (int j=0; j<640; j+=5) {
            if (test[i*j] == 0)
                std::cout << "x";
            else if (test[i*j] > 0 && test[i*j] <=500)
                std::cout << ".";
            else if (test[i*j] > 500 && test[i*j] <=1000)
                std::cout << "o";
            else if (test[i*j] > 1000 && test[i*j] <=1500)
                std::cout << "O";
            else if (test[i*j] > 1500 && test[i*j] <=2000)
                std::cout << "0";
            else
                std::cout << "X";
        }
        std::cout << "\n";
    }


    for (int i=0; i<480; i+=5) {
        for (int j=0; j<640; j+=5) {
            std::cout << "\b";
        }
        std::cout << "\b";
    }
    //for (int i=0; i<5; i++)
        //std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    //std::system("clear");
}

bool DataControl::errorCheck() {
    if (freenect_process_events(ctx) >= 0)
        return true;
    return false;
}

DataControl::DataControl()
{

    //Initialize the library
	int ret = freenect_init(&ctx, NULL);
	if (ret < 0) {
        DataControl::ready = false;
        return;
	}
	std::cout << ret << ": Initialized library.\n";

    //freenect_set_log_level(ctx, FREENECT_LOG_DEBUG);
    freenect_select_subdevices(ctx, FREENECT_DEVICE_CAMERA);

    //Check if there are devices to connect to
    ret = freenect_num_devices(ctx);
    if (ret < 0) {
        DataControl::ready = false;
        return;
	}
	else if (ret == 0) {
        DataControl::ready = false;
        std::cout << "No Devices were detected\n";
	}
	std::cout << ret << ": Found devices.\n";

    //Open the camera device
	ret = freenect_open_device(ctx, &dev, 0);
	if (ret < 0) {
		freenect_shutdown(ctx);
		DataControl::ready = false;
		return;
	}
	std::cout << ret << ": Device opened.\n";

    //Set the callback for retrieving data
    freenect_set_depth_callback(dev, localCallback);
    std::cout << "X: Callback set.\n";

    //Set the debth options
	ret = freenect_set_depth_mode( dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_MM));
	if (ret < 0) {
		freenect_shutdown(ctx);
		freenect_close_device(dev);
		DataControl::ready = false;
		return;
	}
    std::cout << ret << ": Set depth mode.\n";

    //Start the data retrival
    ret = freenect_start_depth(dev);
    if (ret < 0) {
        freenect_close_device(dev);
		freenect_shutdown(ctx);
		DataControl::ready = false;
		return;
	}
	std::cout << ret << ": Started debth.\n";

    //Flag to indicated the device is ready and running
    DataControl::ready = true;
    std::cout << "X: Finished initializing the device\n";

    //while (freenect_process_events(ctx) >= 0) {}

}

//Closes down the connect when the object is destroyed
DataControl::~DataControl()
{
    std::cout << "X: Device is shutting down!\n";
    DataControl::ready = false;

    if (dev != nullptr) {
        freenect_stop_depth(dev);
        freenect_close_device(dev);
    }
    if (ctx != nullptr)
        freenect_shutdown(ctx);
}

