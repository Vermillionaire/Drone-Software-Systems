#include "DataControl.h"
#include "Log.h"
#include <thread>
#include <chrono>
#include "boost/bind.hpp"


using namespace std;

SpinWrapper DataControl::buffer_io;
//fstream DataControl::file("/dev/ttyPS0", ios::in | ios::out | ios::app);
//boost::asio::io_service DataControl::ios;
//boost::asio::serial_port DataControl::sp(ios, "/dev/ttyPS0");
//unsigned char DataControl::angle_buff[20];

long DataControl::frames = 0;
int DataControl::flimiter = 5;

/*
bool ready = false;
long DataControl::frames = 0;
long DataControl::frameLimiter = 0;

double DataControl::timer = 0.0;
int DataControl::tcount = 0;
double DataControl::gett = 0.0;
int DataControl::gcount = 0;
double DataControl::ctimer = 0.0;
int DataControl::ccount = 0;
*/

void DataControl::localCallback(freenect_device *ldev, void *data, uint32_t tm) {

  DataControl::frames++;
  Constants constants;

  SpinArray* sa = buffer_io.putterArray();
  short * fm = (short*) data;
  sa->put(fm, constants.FRAME_WIDTH, constants.FRAME_HEIGHT);

  if (sa->didPutOverflow())
    DataControl::flimiter += 10;

  std::this_thread::sleep_for(std::chrono::milliseconds(30));
}


bool DataControl::errorCheck() {
    if (freenect_process_events(ctx) >= 0 && ready) {
        error_state = true;
        return true;
      }

    if (error_state)
      return true;
    else
      return false;
}

bool DataControl::isReady() {
  return ready;
}

void DataControl::kill() {
  ready = false;
  coprocessor->running = false;
}


DataControl::DataControl() {

  coprocessor = new DataProcessing(&DataControl::buffer_io);
  Constants C;
  //boost::asio::serial_port sp(ios, "/dev/ttyPS0");
  //using namespace boost::asio;
  //sp.set_option(boost::asio::serial_port::baud_rate(115200));
  //boost::asio::async_read(sp,boost::asio::buffer(angle_buff, 20), boost::asio::transfer_at_least(5), boost::bind(&DataControl::serial_callback, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
//  Log::outln(sp.is_open(), "Serial is open.");

    //Initialize the library
	int ret = freenect_init(&ctx, NULL);
	if (ret < 0) {
        ready = false;
        return;
	}
	Log::outln(ret, "Initialized library.");

  freenect_set_log_level(ctx, FREENECT_LOG_ERROR);

  //freenect_device_flags test = (freenect_device_flags) FREENECT_DEVICE_CAMERA | FREENECT_DEVICE_MOTOR;
  freenect_select_subdevices(ctx, (freenect_device_flags) 0x03);

  //Check if there are devices to connect to
  ret = freenect_num_devices(ctx);
  if (ret < 0) {
      ready = false;
      return;
    }
	else if (ret == 0) {
        ready = false;
        Log::outln("No Devices were detected");
        return;
	}
	Log::outln(ret, "Found devices.");

    //Open the camera device
	ret = freenect_open_device(ctx, &dev, 0);
	if (ret < 0) {
		ready = false;
		return;
	}
	Log::outln(ret, "Device opened.");

  //Set IR brightness to max
  ret = freenect_set_ir_brightness(dev, C.IR_BIRGHTNESS);
  Log::outln(ret, "Brightness set to " + std::to_string(C.IR_BIRGHTNESS) );

  //Set the callback for retrieving data
  freenect_set_depth_callback(dev, localCallback);
  Log::outln("Callback set.");

    //Set the debth options
	ret = freenect_set_depth_mode( dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_MM));
	if (ret < 0) {
		//freenect_shutdown(ctx);
		//freenect_close_device(dev);
		ready = false;
		return;
	}
  Log::outln(ret, "Set depth mode.");

  //Start the data retrival
  ret = freenect_start_depth(dev);
  if (ret < 0) {
     //freenect_close_device(dev);
	//freenect_shutdown(ctx);
	ready = false;
	return;
  }
	Log::outln(ret, "Started debth.");

  //Flag to indicated the device is ready and running
  ready = true;
  Log::outln("Finished initializing the camera device");

  ret = coprocessor->epiphanyInit();
  if (ret != 0)
    coprocessor->epiphanyClose();

  coprocessor->startThread();


}

/*
void DataControl::serial_callback(const boost::system::error_code& error, std::size_t bytes_transferred) {
  cout << "Read " << bytes_transferred << " bytes" << endl;
}*/

//Closes down the connect when the object is destroyed
DataControl::~DataControl() {
    Log::outln("Device is shutting down!");

    if (dev != nullptr) {
      freenect_close_device(dev);
    }
    if (ctx != nullptr)
      freenect_shutdown(ctx);

      coprocessor->join();
  		coprocessor->epiphanyClose();

}

int DataControl::clean_restart() {
  std::cout << "Cleaning up old freenect context." << std::endl;

  Constants C;

  if (dev != nullptr)
    freenect_close_device(dev);

  if (ctx != nullptr)
    freenect_shutdown(ctx);

    std::cout << "Recreating freenect context." << std::endl;

    int ret = freenect_init(&ctx, NULL);
  	if (ret < 0)
          return ret;

  	Log::outln(ret, "Initialized library.");

    freenect_set_log_level(ctx, FREENECT_LOG_ERROR);

    //freenect_device_flags test = (freenect_device_flags) FREENECT_DEVICE_CAMERA | FREENECT_DEVICE_MOTOR;
    freenect_select_subdevices(ctx, (freenect_device_flags) 0x03);

    //Check if there are devices to connect to
    ret = freenect_num_devices(ctx);
    if (ret < 0)
        return ret;
  	else if (ret == 0) {
          Log::outln("No Devices were detected");
          return -1;
  	}

  	Log::outln(ret, "Found devices.");

    //Open the camera device
  	ret = freenect_open_device(ctx, &dev, 0);
  	if (ret < 0)
  		return -1;

  	Log::outln(ret, "Device opened.");

    //Set IR brightness to max
    ret = freenect_set_ir_brightness(dev, C.IR_BIRGHTNESS);
    Log::outln(ret, "Brightness set to " + std::to_string(C.IR_BIRGHTNESS) );

    //Set the callback for retrieving data
    freenect_set_depth_callback(dev, localCallback);
    Log::outln("Callback set.");

      //Set the debth options
  	ret = freenect_set_depth_mode( dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_MM));
  	if (ret < 0)
  		return ret;

    Log::outln(ret, "Set depth mode.");

    //Start the data retrival
    ret = freenect_start_depth(dev);
    if (ret < 0)
  	 return ret;

  	Log::outln(ret, "Started debth.");

    //Flag to indicated the device is ready and running
    error_state = false;
    Log::outln("Finished reinitializing the device");

    return 0;
}
