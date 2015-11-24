#include <DataStorage.h>
#include <iostream>
#include <fstream>

using namespace std;

DataStorage::DataStorage() {
  /*
  cloud = new pcl::PointCloud<pcl::PointXYZ>();

  // Fill in the cloud data
  cloud->width    = 640*5;
  cloud->height   = 480*5;
  cloud->is_dense = false;
  cloud->points.resize (cloud->width * cloud->height);
  */

  counter = 0;
  file_num = 0;
  buffer = "";

};

DataStorage::~DataStorage() {
  //std::cout << "Exporting data to file.";
  //pcl::io::savePCDFileASCII("test_pcd.pcd", *cloud);
  //delete cloud;
  writeFile();
}

void DataStorage::writeToFileBuffer(int x, int y, int z) {
  std::lock_guard<std::mutex> lock(mutex);
  if (z < 0)
    return;

  buffer += to_string(x) + " " + to_string(y) + " " + to_string(z) + " 1\n";
  /*
  cloud->points[counter].x = x;
  cloud->points[counter].y = y;
  cloud->points[counter].z = z;
  */
  counter++;
}


bool DataStorage::writeFile() {
  if (counter <= 0)
    return false;

  ofstream myfile;
  myfile.open("Scan-Number-" + to_string(file_num) + ".txt");
  myfile << "0\n";
  myfile << counter << "\n";
  myfile << "0 0 0\n";
  myfile << "1 0 0\n";
  myfile << "0 1 0\n";
  myfile << "0 0 1\n";
  myfile << "0 0 0\n";
  myfile << "1 0 0 0\n";
  myfile << "0 1 0 0\n";
  myfile << "0 0 1 0\n";
  myfile << "0 0 0 1\n";
  myfile << buffer;
  myfile.close();

  counter = 0;
  buffer = "";
  return true;
}

int DataStorage::getBufferLength() {
  return buffer.length();
}
