#include "Kinect.h"
#include <iostream>
#include <stdlib.h>

Kinect::Kinect()
{

	int ret = freenect_init(&ctx, NULL);
	if (ret < 0) {
        ready = false;
        return;
	}

    freenect_select_subdevices(ctx, FREENECT_DEVICE_CAMERA);

    ret = freenect_num_devices(ctx);
    if (ret < 0) {
        ready = false;
        return;
	}
	else if (ret == 0) {
        ready = false;
        std::cout << "No Devices were detected\n";
	}

	ret = freenect_open_device(ctx, &dev, 0);
	if (ret < 0) {
		freenect_shutdown(ctx);
		ready = false;
		return;
	}

	ret = freenect_set_depth_mode( dev, freenect_find_depth_mode(FREENECT_RESOLUTION_HIGH, FREENECT_DEPTH_MM));
	if (ret < 0) {
		freenect_shutdown(ctx);
		ready = false;
		return;
	}

    freenect_set_depth_callback(dev, localCallback);

}

Kinect::~Kinect()
{
    //dtor
}

void Kinect::setDebthCallback(void (*callback)(void*,uint32_t)) {
    generic_cb = callback;

}


void Kinect::localCallback(freenect_device *ldev, void *data, uint32_t time) {
    std::cout << "Getting stuff";
}
