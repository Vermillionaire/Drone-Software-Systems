//#include <iostream> // std::cout
//#include <pcl/io/pcd_io.h>
//#include <pcl/point_types.h>

#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include "Log.h"

class DataStorage {
	public:
		DataStorage();
		~DataStorage();

		void writeToFileBuffer(short* frame, int width, int height, int angle);
		void writeToFileBuffer(std::string s);
		bool writeFile();
		int getBufferLength();

	private:
    //pcl::PointCloud<pcl::PointXYZ> *cloud;
    int counter;
		int file_num;
		std::string buffer;
		std::mutex mutex;


};

#endif
