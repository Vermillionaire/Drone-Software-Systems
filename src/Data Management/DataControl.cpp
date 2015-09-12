#include "DataControl.h"
#include "Log.h"
#include <iostream>

SpinArray DataControl::buff(DataControl::width*DataControl::height*600);
bool DataControl::ready = false;
long DataControl::frames = 0;

void DataControl::localCallback(freenect_device *ldev, void *data, uint32_t time) {
    //std::cout << "Getting stuff\r";
    //DataControl::ready = false;
    DataControl::frames++;
    //std::cout << frames << std::endl;
    
    short* dataC = (short*)data;

    
    for (int i=0; i<480; i++) {
        for (int j=0; j<640; j++) {
            if (dataC[i*j] != 0) {
                SpinArray::DPoint* point = new SpinArray::DPoint;

                point->depth = dataC[i*j];
                point->i = i;
                point->j = j;

                buff.put( point );
           }
        }
    }
    
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
	Log::outln(ret, "Initialized library.");

    freenect_set_log_level(ctx, FREENECT_LOG_ERROR);
    
    //freenect_device_flags test = (freenect_device_flags) FREENECT_DEVICE_CAMERA | FREENECT_DEVICE_MOTOR;
    freenect_select_subdevices(ctx, (freenect_device_flags) 0x03);

    //Check if there are devices to connect to
    ret = freenect_num_devices(ctx);
    if (ret < 0) {
        DataControl::ready = false;
        return;
	}
	else if (ret == 0) {
        DataControl::ready = false;
        Log::outln("No Devices were detected");
        return;
	}
	Log::outln(ret, "Found devices.");

    //Open the camera device
	ret = freenect_open_device(ctx, &dev, 0);
	if (ret < 0) {
		DataControl::ready = false;
		return;
	}
	Log::outln(ret, "Device opened.");

    //Set IR brightness to max
    ret = freenect_set_ir_brightness(dev, 50);
    Log::outln(ret, "Brightness set to " + std::to_string(DataControl::brightness) );


    //Set the callback for retrieving data
    freenect_set_depth_callback(dev, localCallback);
    Log::outln("Callback set.");

    //Set the debth options
	ret = freenect_set_depth_mode( dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_MM));
	if (ret < 0) {
		//freenect_shutdown(ctx);
		//freenect_close_device(dev);
		DataControl::ready = false;
		return;
	}
    Log::outln(ret, "Set depth mode.");

    //Start the data retrival
    ret = freenect_start_depth(dev);
    if (ret < 0) {
       //freenect_close_device(dev);
		//freenect_shutdown(ctx);
		DataControl::ready = false;
		return;
	}
	Log::outln(ret, "Started debth.");

    //Flag to indicated the device is ready and running
    DataControl::ready = true;
    Log::outln("Finished initializing the device");


}

//Closes down the connect when the object is destroyed
DataControl::~DataControl()
{
    Log::outln("Device is shutting down!");

    if (dev != nullptr) {
        freenect_stop_depth(dev);
        freenect_close_device(dev);
    }
    if (ctx != nullptr)
        freenect_shutdown(ctx);
}

