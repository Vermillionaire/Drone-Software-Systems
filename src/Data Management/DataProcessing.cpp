#include "DataProcessing.h"
#include "DataControl.h"
#include "SpinArray.h"
#include "Log.h"
#include <chrono>
#include <utility>
#include <string>

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
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//std::this_thread::yield();
	Log::outln(id, "Thread # starting.");
	std::string output("{ \"size\": \"300\",\n \"data\": [{\n\t");


	int size = 1000;
	int counter = 1;
	Point * p = new Point[size];

	do {
		std::cout << "Doing calculations...\n";
		//std::cout << "Ready? " << DataControl::ready ;
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));

		//SpinArray::DPoint * point = DataControl::buff.get();

		DataControl::buff.get(p, size);

		//Point* p = new Point();
		int num = 0;
		for (int i=0; i<size; i++) {

			if (p[i].z == 0)
				continue;

			num++;
			output += "{\n";
			//output += "\"point\": [{\n\t\t";
			output += " \"x\": \"" + std::to_string( (int)((p[i].x - w2) * (p[i].z + minDistance) * scaleFactor)/10 ) + "\",\n\t\t";
			output += " \"y\": \"" + std::to_string( (int)((p[i].y - h2) * (p[i].z + minDistance) * scaleFactor)/10 ) + "\",\n\t\t";
			output += " \"z\": \"" + std::to_string( (int)(p[i].z / 10.0) ) + "\"\n\t";
			//output += "}],\n\t";
			output += "},\n";
			/*
			p[i].x = (int)((float)((p[i].x - w2) * (p[i].z + minDistance)) * scaleFactor)/10;
			p[i].y = (int)((float)((p[i].y - h2) * (p[i].z + minDistance)) * scaleFactor)/10;
			p[i].z = p[i].z / 10.0;
			*/
		}

		if (num <= 0)
			continue;

		output += "\"end\": \"end\"\n\t}]\n}";
		Log::fileOut("data"+std::to_string(counter++)+".json", output);

		//std::make_pair<Point,Point>(p,p);
		//mymap.insert(std::make_pair(*p,*p));


	} while (DataControl::ready || DataControl::buff.getDistance() > 0);

	std::cout << "Test" << DataControl::buff.getDistance();
	DataControl::buff.printSize();

	//for (int i=0; i<size; i++) {
	//	n.write(p[i]);
//	}
	Log::outln(id, "Thread # finished.");
	delete[] p;

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
