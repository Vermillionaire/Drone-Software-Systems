#include "DataProcessing.h"
#include "DataControl.h"
#include "SpinArray.h"
#include "Log.h"
#include <chrono>
#include <utility>

const int minDistance = -10;
const float scaleFactor = .0021;

//half width and height
const int w2 = 320;
const int h2 = 240;

DataProcessing::DataProcessing() {
	Log::outln("Starting data processing.");
	thread1 = new std::thread(&DataProcessing::compute, this, 1);
	//thread2 = new std::thread(&DataProcessing::compute, this, 2);
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
	//std::this_thread::yield();
	Log::outln(id, "Thread # starting.");

	int size = 300;
	do {
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1));

		//SpinArray::DPoint * point = DataControl::buff.get();
		Point * point = new Point[size];
		DataControl::buff.get(point, size);


		//if (point == nullptr)
			//continue;

		//Point* p = new Point();
		for (int i=0; i<size; i++) {
			p[i].x = (int)((float)((p[i].i - w2) * (p[i].z + minDistance)) * scaleFactor)/10;
			p[i].y = (int)((float)((p[i].j - h2) * (p[i].z + minDistance)) * scaleFactor)/10;
			p[i].z = p[i].z / 10.0;
		}

		//std::make_pair<Point,Point>(p,p);
		//mymap.insert(std::make_pair(*p,*p));
		delete[] point;


	} while (DataControl::ready);

	Log::outln(id, "Thread # finished.");

};

void DataProcessing::consume(int id) {
	while ( DataControl::ready) {
		//std::chrono::milliseconds(10);
		//std::cout << "Consumer " << id << "consuming.\n";
		DataControl::buff.get();
	}
};

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
	thread1->join();
	//thread2->join();
	//thread3->join();
	fpsc->join();
	Log::outln("All threads finished.");
};
