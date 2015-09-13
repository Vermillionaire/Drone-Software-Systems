#include "DataProcessing.h"
#include "DataControl.h"
#include "SpinArray.h"
#include "Log.h"
#include <chrono>

const int minDistance = -10;
const float scaleFactor = .0021;

//half width and height
const int w2 = 320;
const int h2 = 240;

DataProcessing::DataProcessing() {
	Log::outln("Starting data processing.");
	//thread1 = new std::thread(&DataProcessing::compute, this, 1);
	//thread2 = new std::thread(&DataProcessing::inspect, this, 2);
	//thread3 = new std::thread(&DataProcessing::compute, this, 3);
	fpsc = new std::thread(&DataProcessing::fpsCounter, this);
	
};

DataProcessing::~DataProcessing() {
	Log::outln("Finished processing.");
	//delete thread1;
	//elete thread2;
	//delete thread3;
	delete fpsc;

};


void DataProcessing::compute(int id) {
	int x,y,z;
	Log::outln(id, "Thread # starting.");
	while (DataControl::ready) {
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));

		SpinArray::DPoint * point = DataControl::buff.get();
	
		if (point == nullptr) 
			continue;

		z = point->depth;
		if (z != 0) {
			x = (int)((float)((point->i - w2) * (z + minDistance)) * scaleFactor);
			y = (int)((float)((point->j - h2) * (z + minDistance)) * scaleFactor);
		}

		delete point;
		
	}
	Log::outln(id, "Thread # finished.");
	
};

void DataProcessing::consume(int id) {
	while ( DataControl::ready) {
		std::chrono::milliseconds(10);
		//std::cout << "Consumer " << id << "consuming.\n";
		delete DataControl::buff.get();
	}
};

void DataProcessing::inspect(int id) {
	//std::this_thread::sleep_for(std::chrono::milliseconds(500));
	int x=0,y=0,z=0;
	Log::outln(id, "Thread # starting.");

	while (DataControl::ready) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		
		SpinArray::DPoint * point = DataControl::buff.get();

		if (point == nullptr) {
			continue;
		}

		z = point->depth;
		if (z != 0) {
			x = (int)((float)((point->i - w2) * (z + minDistance)) * scaleFactor);
			y = (int)((float)((point->j - h2) * (z + minDistance)) * scaleFactor);
		}

		std::cout << "-----------\n";
		std::cout << point->i << " " << point->j << " " << point->depth << std::endl;
		delete point;
		std::cout << x << " " << y << " " << z << std::endl;
	}

	Log::outln(id, "Thread # finished.");
}

void DataProcessing::fpsCounter() {

	while (DataControl::ready) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		DataControl::buff.printSize();
		std::cout << " FPS:" << DataControl::frames << std::endl;
		DataControl::frames = 0;

	}
}

void DataProcessing::joinAll() {
	Log::outln("Waiting for threads...");
	//thread1->join();
	//thread2->join();
	//thread3->join();
	fpsc->join();
	Log::outln("All threads finished.");
};