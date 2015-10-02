#include "DataProcessing.h"
#include <chrono>
#include <utility>


const int minDistance = -10;
const float scaleFactor = .0021;
//half width and height
const int w2 = 320;
const int h2 = 240;

DataProcessing::DataProcessing() { Log::outln("Data processing init."); };

DataProcessing::~DataProcessing() {
	Log::outln("Finished processing.");

	delete thread1;
	//delete fpsc;

};

void DataProcessing::startThread() {
	thread1 = new std::thread(&DataProcessing::epiphanyRun, this);
	//fpsc = new std::thread(&DataProcessing::fpsCounter, this);
}

//Need to get header file to compile on this system
//Make sure Home is set: EPIPHANY_HDF=”${EPIPHANY_HOME}/bsps/parallella/parallella.xml”
void DataProcessing::epiphanyInit() {
	std::cout << "Starting Epiphany device!" << std::endl;

	int ret = 0;
	e_platform_t platform;
	dev = new e_epiphany_t;

	e_set_host_verbosity(H_D0);

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

	//Opens the group. Get row and col info from platform
	ret = e_open(dev, 0, 0, 4, 4);
	if (ret == E_OK)
		std::cout << "Opened Fine!" << std::endl;
	else {
		std::cout << "Did not open!" << std::endl;
		e_finalize();
		return;
	}

	ret = e_load_group("e_calc.srec", dev, 0, 0, 4, 4, E_FALSE);
	if (ret == E_OK)
		std::cout << "Loaded Program!" << std::endl;
	else {
		std::cout << "Failed to load Program!" << std::endl;
		e_finalize();
		return;
	}
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

void DataProcessing::epiphanyRun() {

	e_mem_t dbug_size;
	e_mem_t dbug_text;
	e_mem_t mbuf;

	//Alocate memory for all sixteen cores
	int data_objects = 5;
	int data_size = sizeof(PointKey)*data_objects;
	int header_size = sizeof(int)*2;
	int num_cores = 16;

	int ret = e_alloc(&mbuf, 0, (header_size + data_size)*num_cores);
	if (ret == E_OK)
		std::cout << "Allocated Memory!" << std::endl;
	else {
		std::cout << "Failed to alocate Memory!" << std::endl;
		e_finalize();
		return;
	}


	PointKey pk;
	pk.p.x = 267;
	pk.p.y = 17;
	pk.p.z = 3186;
	pk.key = 0;

	PointKey pArray[data_objects];



	for (int i=0; i<data_objects; i++)
		pArray[i] = pk;

	std::cout << 0 << ": Status Code." << std::endl;
	std::cout << data_objects << ": Size." << std::endl;
	std::cout << pArray[0].p.x << ": X." << std::endl;
	std::cout << pArray[0].p.y << ": Y." << std::endl;
	std::cout << pArray[0].p.z << ": Z." << std::endl;
	std::cout << pArray[0].key << ": Hash." << std::endl;


	for (int i=0; i<num_cores; i++) {
		int init_code = 0;
		int offset = (header_size + data_size)*i;
		e_write(&mbuf, 0, 0, offset, &init_code, 4);
		e_write(&mbuf, 0, 0, offset + 4, &data_objects, 4);
		e_write(&mbuf, 0, 0, offset + 8, &pArray, data_size);

		std::cout << "I = " << i << " Offset = " << offset << std::endl;
	}

	bool not_done = false;
	while (not_done) {
		/* code */
	}

	e_free(&mbuf);
	e_free(&dbug_size);
	e_free(&dbug_text);

	std::cout << "Done running" << std::endl;

/*
	auto start = std::chrono::high_resolution_clock::now();

	e_write(mbuf, 0, 0, 0, &code, 4);
	e_write(mbuf, 0, 0, 4, &size, 4);
	e_write(mbuf, 0, 0, 8, &pArray, sizeof(PointKey)*size);

	auto time1 = std::chrono::high_resolution_clock::now();


	ret = e_start(dev, 0, 0);
	if (ret == E_OK)
		std::cout << "Started!" << std::endl;
	else {
		std::cout << "Failed to start!" << std::endl;
		e_finalize();
		return;
	}
	while (j!=-1 && c < 10000) {
		e_read(mbuf, 0, 0, 0, &j, 4);
		c++;
	}

	auto time2 = std::chrono::high_resolution_clock::now();

	e_read(mbuf, 0, 0, 0, &code, 4);
	e_read(mbuf, 0, 0, 4, &size, 4);
	e_read(mbuf, 0, 0, 8, &pArray, sizeof(PointKey));

	auto finish = std::chrono::high_resolution_clock::now();
	long timer1 = 0;
	long timer2 = 0;
	long timer3 = 0;
	timer1 = std::chrono::duration_cast<std::chrono::nanoseconds>(time1-start).count();
	timer2 = std::chrono::duration_cast<std::chrono::nanoseconds>(time2-start).count();
	timer3 = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

	std::cout << c << ": After" << std::endl;
	std::cout << code << ": Status Code." << std::endl;
	std::cout << size << ": Size." << std::endl;
	std::cout << pArray[0].p.x << ": X." << std::endl;
	std::cout << pArray[0].p.y << ": Y." << std::endl;
	std::cout << pArray[0].p.z << ": Z." << std::endl;
	std::cout << pArray[0].key << ": Hash." << std::endl;
	std::cout << timer1 << ": Time Write." << std::endl;
	std::cout << timer2 << ": Time Start." << std::endl;
	std::cout << timer3 << ": Time Read." << std::endl;

	e_alloc(&dbug_size, 0x00100000 , sizeof(int)*3);
	int text_size = 0;
	int core_row = 0;
	int core_col = 0;
	e_read(&dbug_size, 0, 0, 0x08, &core_col, sizeof(int));
	e_read(&dbug_size, 0, 0, 0x04, &core_row, sizeof(int));
	e_read(&dbug_size, 0, 0, 0x00, &text_size, sizeof(int));

	std::cout << text_size << ": Size dbug." << std::endl;

	e_alloc(&dbug_text, 0x0010000c, sizeof(char)*text_size);
	char text[text_size];
	e_read(&dbug_text, 0, 0, 0, &text, sizeof(char)*text_size);

	std::cout << "//////Core " << core_row << "," << core_col << " Debug//////" << std::endl;
	for (int i=0; i<text_size; i++)
		std::cout << text[i];
	std::cout << "////////////////////////////" << std::endl;
	*/

	//e_read(mbuf, 0, 0, 4, &j, 4);
	//std::cout << j << ": Read test" << std::endl;
	//j = 0;

}

void DataProcessing::epiphanyClose() {
	int ret = 0;


	ret = e_close(dev);
	Log::outln(ret,"Core group closed.");
	ret = e_finalize();
	Log::outln(ret,"Epiphany closed.");

	delete dev;
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
			std::cout << p[i].x << ", " << p[i].y << ", " << p[i].z << std::endl;
			//p[i].x = (int)((float)((p[i].x - w2) * (p[i].z + minDistance)) * scaleFactor)/10;
			//p[i].y = (int)((float)((p[i].y - h2) * (p[i].z + minDistance)) * scaleFactor)/10;
			//p[i].z = p[i].z / 10.0;
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

void DataProcessing::join() {
	Log::outln("Waiting for threads...");
	thread1->join();
	//fpsc->join();
	Log::outln("All threads finished.");
};
