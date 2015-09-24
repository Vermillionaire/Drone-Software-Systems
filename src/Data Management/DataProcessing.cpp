#include "DataProcessing.h"
#include <chrono>
#include <utility>

const int minDistance = -10;
const float scaleFactor = .0021;
//half width and height
const int w2 = 320;
const int h2 = 240;

DataProcessing::DataProcessing() {
	Log::outln("Data processing init.");
	//fpsc = new std::thread(&DataProcessing::fpsCounter, this);

};

DataProcessing::~DataProcessing() {
	Log::outln("Finished processing.");
	delete thread1;

	delete fpsc;

};

void DataProcessing::startThreads() {
	thread1 = new std::thread(&DataProcessing::compute, this, 1);
}

//Need to get header file to compile on this system
//Make sure Home is set: EPIPHANY_HDF=”${EPIPHANY_HOME}/bsps/parallella/parallella.xml”
void DataProcessing::epiphanyInit() {
	std::cout << "Starting Epiphany device!" << std::endl;

	int ret = 0;
	e_platform_t platform;
	e_epiphany_t dev;

	e_set_host_verbosity(H_D4);

	ret = e_init(NULL);
	if (ret == E_OK)
		std::cout << "Init sucessfull!" << std::endl;
	else {
		std::cout << "Failed to start!" << std::endl;
		e_finalize();
		return;
	}

	ret = e_reset_system();
	if (ret == E_OK)
		std::cout << "Reset!" << std::endl;
	else {
		std::cout << "Couldn't reset?" << std::endl;
		e_finalize();
		return;
	}

	ret = e_get_platform_info(&platform);
	if (ret == E_OK)
		std::cout << "Got the info!" << std::endl;
	else {
		std::cout << "No info :(" << std::endl;
		e_finalize();
		return;
	}

	std::cout << "Starting Epiphany cores!" << std::endl;
	std::cout << "-Version: " << platform.version << std::endl;
	std::cout << "-Coordinates: (" << platform.row << "," << platform.col << ")" << std::endl;
	std::cout << "-Area: " << platform.rows << "x" << platform.cols << std::endl;

	int chips = platform.num_chips;


	//Opens the group. Get row and col info from platform
	ret = e_open(&dev, 0, 0, 4, 4);
	if (ret == E_OK)
		std::cout << "Opened Fine!" << std::endl;
	else {
		std::cout << "Did not open!" << std::endl;
		e_finalize();
		return;
	}

	std::cout << "Testing open!" << std::endl;
	std::cout << dev.base_coreid << std::endl;

	//Alocate memory
	mbuf = new e_mem_t;
	//e_alloc(mbuf, 0, (CORE_BUFFER_SIZE+sizeof(int))*chips);

	ret = e_alloc(mbuf, 0, sizeof(int)*2);
	if (ret == E_OK)
		std::cout << "Allocated Memory!" << std::endl;
	else {
		std::cout << "Failed to alocate Memory!" << std::endl;
		e_finalize();
		return;
	}

	int test = 1;
	ret = e_write(mbuf, 0, 0, 0, &test, sizeof(int));
	if (ret > 0)
		std::cout << "Wrote to memory!" << std::endl;
	else {
		std::cout << "Memory write failed!" << std::endl;
		e_finalize();
		return;
	}

	//ret = e_load("/home/parallella/Projects/Drone-Software-Systems/src/Epiphany/e_calc.elf", &dev, 0, 0, E_TRUE);
	if (ret == E_OK)
		std::cout << "Loaded Program!" << std::endl;
	else {
		std::cout << "Failed to load Program!" << std::endl;
		e_finalize();
		return;
	}

	std::cout << "Free!" << std::endl;
	e_free(mbuf);

	//ssize_t e_read(void *dev, unsigned row, unsigned col, off_t from_addr, void *buf, size_t size);
	//ssize_t e_write(void *dev, unsigned row, unsigned col, off_t to_addr, const void *buf, size_t size);

	std::cout << "Close!" << std::endl;
	e_close(&dev);
	std::cout << "Finalize!" << std::endl;
	e_finalize();

	/*
	int e_reset_system();
	int e_reset_group(e_epiphany_t *dev);
	int e_start(e_epiphany_t *dev, unsigned row, unsigned col);
	int e_start_group(e_epiphany_t *dev);
	int e_signal(e_epiphany_t *dev, unsigned row, unsigned col);
	int e_halt(e_epiphany_t *dev, unsigned row, unsigned col);
	int e_resume(e_epiphany_t *dev, unsigned row, unsigned col);

	int e_load(char *executable, e_epiphany_t *dev, unsigned row, unsigned col, e_bool_t start);
	googint e_load_group(char *executable, e_epiphany_t *dev, unsigned row, unsigned col, unsigned rows, unsigned cols, e_bool_t start);
	*/
}


void DataProcessing::compute(int id) {
	//std::this_thread::yield();
	Log::outln(id, "Thread # starting.");

	int size = 300;
	do {
		//std::cout << "Processing 300 points\n";
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1));

		//SpinArray::DPoint * point = DataControl::buff.get();

		auto start = std::chrono::high_resolution_clock::now();
		Point * p = new Point[size];
		DataControl::buff.get(p, size);
		auto finish = std::chrono::high_resolution_clock::now();
		DataControl::gett += std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
		DataControl::gcount++;

		start = std::chrono::high_resolution_clock::now();
		//Point * p = new Point[size];
		for (int i=0; i<size; i++) {
			p[i].x = (int)((float)((p[i].x - w2) * (p[i].z + minDistance)) * scaleFactor)/10;
			p[i].y = (int)((float)((p[i].y - h2) * (p[i].z + minDistance)) * scaleFactor)/10;
			p[i].z = p[i].z / 10.0;
		}

		//std::make_pair<Point,Point>(p,p);
		//mymap.insert(std::make_pair(*p,*p));
		delete[] p;
		finish = std::chrono::high_resolution_clock::now();
		DataControl::ctimer += std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
		DataControl::ccount++;

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

	fpsc->join();
	Log::outln("All threads finished.");
};
