//#include <iostream> // std::cout
//#include <pcl/io/pcd_io.h>
//#include <pcl/point_types.h>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>

class DataStorage {
	public:
		DataStorage();
		~DataStorage();

    void writeToFileBuffer(int x, int y, int z);
		bool writeFile();
		int getBufferLength();

	private:
    //pcl::PointCloud<pcl::PointXYZ> *cloud;
    int counter;
		int file_num;
		std::string buffer;
		std::mutex mutex;


};
