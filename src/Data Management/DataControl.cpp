#include "DataControl.h"
#include "Log.h"
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "boost/bind.hpp"


using namespace std;

//SpinArray DataControl::buffer;
//fstream DataControl::file("/dev/ttyPS0", ios::in | ios::out | ios::app);
//boost::asio::io_service DataControl::ios;
//boost::asio::serial_port DataControl::sp(ios, "/dev/ttyPS0");
//unsigned char DataControl::angle_buff[20];

long DataControl::frames = 0;
int DataControl::flimiter = 5;
int DataControl::angle = 0;
bool DataControl::recording = false;
DataStorage* DataControl::store;

//SpinArray* DataControl::buffer = new SpinArray(640*480*120);
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

  if (DataControl::recording) {
    //DataControl::frames++;
    //Constants constants;

    //SpinArray* sa = buffer_io.putterArray();
    short * fm = (short*) data;
    //DataControl::buffer->put(fm, constants.FRAME_WIDTH, constants.FRAME_HEIGHT, angle);
    store->writeToFileBuffer(fm, 640, 480, DataControl::angle);

    //if (DataControl::buffer->didPutOverflow())
    //  DataControl::flimiter += 10;

    //std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
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
  //coprocessor->running = false;
}


DataControl::DataControl() {

//  coprocessor = new DataProcessing();
  //Constants C;
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
  ret = freenect_set_ir_brightness(dev, 50);
  Log::outln(ret, "Brightness set to " + std::to_string(50) );

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

  DataControl::store = new DataStorage();
  //serial_thread = new std::thread(&DataControl::serial_thread_func, this);


  DataControl::recording = true;
/*
  ret = coprocessor->epiphanyInit();
  if (ret != 0)
    coprocessor->epiphanyClose();

  DataControl::recording = true;
  coprocessor->startThread();
  */


}

/*
void DataControl::serial_callback(const boost::system::error_code& error, std::size_t bytes_transferred) {
  cout << "Read " << bytes_transferred << " bytes" << endl;
}*/

//Closes down the connect when the object is destroyed
DataControl::~DataControl() {
    Log::outln("Device is shutting down!");

    serial_thread_running = false;

    if (dev != nullptr) {
      freenect_close_device(dev);
    }
    if (ctx != nullptr)
      freenect_shutdown(ctx);

      store->writeFile();
      //coprocessor->join();
  		//coprocessor->epiphanyClose();

}

int DataControl::clean_restart() {
  std::cout << "Cleaning up old freenect context." << std::endl;

  //Constants C;

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
    ret = freenect_set_ir_brightness(dev, 50);
    Log::outln(ret, "Brightness set to " + std::to_string(50) );

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

void DataControl::serial_thread_func() {
  serial_thread_running = true;

  Log::outln("Opening serial port.");
	int serial_fd = open("/dev/ttyPS0", O_RDONLY | O_NOCTTY);

	unsigned speed = 115200;

	if (serial_fd < 0) {
		Log::outln("Could not open serial port!");
    return;
	}

	/* Try to set baud rate */
	struct termios uart_config;
	int termios_state;

  Log::outln("Setting configurations.");
	/* Back up the original uart configuration to restore it after exit */
	if ((termios_state = tcgetattr(serial_fd, &uart_config)) < 0) {
    Log::outln("Serial port config error!");
		close(serial_fd);
		return;
	}

  cfsetispeed(&uart_config, B115200);
  cfsetospeed(&uart_config, B115200);

	/* Clear ONLCR flag (which appends a CR for every LF)
	uart_config.c_oflag &= ~ONLCR;


	if (cfsetispeed(&uart_config, speed) < 0 || cfsetospeed(&uart_config, speed) < 0) {
    Log::outln("Could not set baud rate!");
		close(serial_fd);
		return;
	}

	if ((termios_state = tcsetattr(serial_fd, TCSANOW, &uart_config)) < 0) {
    Log::outln("Could not set more configs!");
		close(serial_fd);
		return;
	}*/
  bool angle90 = false;
  bool angle180 = false;
  bool angle270 = false;
  bool angle360 = false;

  unsigned char buff[8];
  for(int i=0; i<8; i++)
    buff[i] = 0;

  Log::outln("Starting read loop.");
  while (serial_thread_running) {
    int num = read(serial_fd, buff, sizeof(buff));
    //Log::outln("Got something.");
  //  struct serial_package pack;

    if (buff[0] != 0xFA) {
      Log::outln("Synchronizing byte stream.");

      printf(" %d %2x %2x %2x %2x %2x %2x %2x %2x ", num, buff[0], buff[1], buff[2], buff[3], buff[4], buff[5], buff[6], buff[7]);

      for (int i=1; i<8; i++) {
        if (buff[i] == 0xFA){
          if (i+1 == 8) {
            char tmp[7];
            num = read(serial_fd, tmp, sizeof(tmp));
          }
          else if (buff[i+1] == 0xFF) {
            char tmp[i];
            num = read(serial_fd, tmp, sizeof(tmp));
            break;
          }
        }
      }
    }
    else {
      DataControl::angle = ((int)buff[5]<<8) + ((int)buff[4]);
      //Log::outln(DataControl::angle, "angle");

      if (DataControl::angle == 0 && !DataControl::recording) {
        Log::outln("Starting recording");
        DataControl::recording = true;
        //coprocessor->running = true;
        //coprocessor->startThread();
      }
      else if(DataControl::angle == 0 && DataControl::recording && angle90 && angle180 && angle270 && angle360) {
        Log::outln("Stopping recording");
        DataControl::recording = false;
        store->writeFile();
        //std::this_thread::sleep_for(std::chrono::milliseconds(30000));
        //coprocessor->running = false;
        //coprocessor->join();
        //coprocessor->store.writeFile();
        angle90 = false;
        angle180 = false;
        angle270 = false;
        angle360 = false;
      }
      else if (DataControl::angle == 90 && DataControl::recording) {
        Log::outln("Hit 90");
        angle90 = true;
      }
      else if (DataControl::angle == 180 && DataControl::recording) {
        Log::outln("Hit 180");
        angle180 = true;
      }
      else if (DataControl::angle == 270 && DataControl::recording) {
        Log::outln("Hit 270");
        angle270 = true;
      }
      else if (DataControl::angle == 359 && DataControl::recording) {
        Log::outln("Hit 359");
        angle360 = true;
      }


    }
  }

}
