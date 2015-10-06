#include "DataProcessing.h"
#include <chrono>
#include <utility>


const int minDistance = -10;
const float scaleFactor = .0021;
//half width and height
const int w2 = 320;
const int h2 = 240;
const int MAX_SIZE = 1000;


DataProcessing::DataProcessing() { Log::outln("Data processing init."); };

DataProcessing::~DataProcessing() {
	Log::outln("Finished processing.");

	delete thread1;
	delete fpsc;

};

void DataProcessing::startThread() {
	thread1 = new std::thread(&DataProcessing::epiphanyRun, this);
	fpsc = new std::thread(&DataProcessing::fpsCounter, this);
}

//Need to get header file to compile on this system
//Make sure Home is set: EPIPHANY_HDF=”${EPIPHANY_HOME}/bsps/parallella/parallella.xml”
int DataProcessing::epiphanyInit() {
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
		return -1;
	}

	ret = e_reset_system();
	if (ret == E_OK)
		std::cout << "Reset!" << std::endl;
	else {
		std::cout << "Couldn't reset?" << std::endl;
		e_finalize();
		return -1;
	}

	ret = e_get_platform_info(&platform);
	if (ret == E_OK)
		std::cout << "Got the info!" << std::endl;
	else {
		std::cout << "No info :(" << std::endl;
		e_finalize();
		return -1;
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
		return -1;
	}

	ret = e_load_group("e_calc.srec", dev, 0, 0, 4, 4, E_FALSE);
	if (ret == E_OK)
		std::cout << "Loaded Program!" << std::endl;
	else {
		std::cout << "Failed to load Program!" << std::endl;
		e_finalize();
		return -1;
	}

	std::cout << "Setting shared memory to all zeros" << std::endl;
	e_mem_t mem;
	int zero = 0;
	int meg32 = 1000*1000*8;

	ret = e_alloc(&mem, 0, meg32*sizeof(int));
	if (ret == E_OK)
		std::cout << "Allocated wipe memory!" << std::endl;
	else {
		std::cout << "Failed to alocate wipe memory!" << std::endl;
		e_finalize();
		return -1;
	}

	//Writes zero value to each value the size of an int, for 32 Mb
	for (int i=0; i<meg32; i++) {
		ret = e_write(&mem, 0, 0, i*4, &zero, 4);
	}

	return 0;
}

void DataProcessing::epiphanyRun() {
	e_mem_t mbuf;

	std::cout << "\n\nStarting Data processing with epiphany cores!\n";

	//Alocate memory for all sixteen cores
	int data_size = sizeof(PointKey)*MAX_SIZE;
	int header_size = sizeof(int)*3;
	int num_cores = 16;

	//std::cout << "Data size = " << header_size << std::endl;

	int ret = e_alloc(&mbuf, 0, (header_size + data_size)*num_cores);
	if (ret == E_OK)
		std::cout << "Allocated Memory!" << std::endl;
	else {
		std::cout << "Failed to alocate Memory!" << std::endl;
		e_finalize();
		return;
	}

	//Sets the host status codes to 2
	int init_code = 2;
	for (int i=0; i<num_cores; i++) {
		int offset = (header_size + data_size)*i;
		ret = e_write(&mbuf, 0, 0, offset, &init_code, 4);

		if (ret < 4)
			std::cout << "Did not write the start codes correctly" << std::endl;
	}

	ret = e_start_group(dev);
	//ret = e_start(dev, 0, 0);
	//ret = e_start(dev, 0, 1);
	//ret = e_start(dev, 0, 2);
	if (ret == E_OK)
		std::cout << "Started!" << std::endl;
	else {
		std::cout << "Failed to start!" << std::endl;
		e_finalize();
		return;
	}

	bool not_done = true;
	bool shutdown = false;
	bool wait = false;
	int num_errors = 0;
	PointKey *pArray = new PointKey[MAX_SIZE];

	//Ststus code loggers
	int code_0 = 0;
	int code_1 = 0;
	int code_2 = 0;
	int code_3 = 0;
	int code_7 = 0;
	int code_8 = 0;
	int code_9 = 0;
	int code_other = 0;

	//Pregenerate the offset codes
	int offset_codes[16];
	for (int i=0; i<16; i++) {
		offset_codes[i] = (header_size + data_size)*i;
		std::cout << "Offset " << i << ": " << std::hex << offset_codes[i] << std::dec << std::endl;
	}

	while (not_done) {
		wait = false;
		//std::cout << "DISTANCE: " << DataControl::buff.getDistance() << std::endl;

		//Reads through the status codes of the cores
		for (int i=0; i<num_cores; i++) {
			int core_code = 0;
			e_read(&mbuf, 0, 0, offset_codes[i] + 4, &core_code, 4);
		//	std::cout << "CORE: "<< i << "|" << core_code << "  ";

			int init_code = 0;
			switch (core_code) {
				case 0:
					code_0++;
					if (!shutdown) {
						//Bad error, faulty core
						num_errors++;
						init_code = 0;
						e_write(&mbuf, 0, 0, offset_codes[i], &init_code, 4);
					}
					break;
				case 1:
					code_1++;
					break;
				case 2:
					init_code = 3;
					e_write(&mbuf, 0, 0, offset_codes[i], &init_code, 4);
					code_2++;
					break;
				case 3:
					code_3++;
					break;
				case 7:
					code_7++;
					break;
				case 8:
					num_errors++;
					init_code = 2;
					e_write(&mbuf, 0, 0, offset_codes[i], &init_code, 4);
					code_8++;
					break;
				case 9:
					wait = true;
					code_9++;
					break;
				default:
					code_other++;
					break;
			}
		}
		//std::cout << std::endl;


		int num_done = 0;
		//Reads through the status codes of the cores
		for (int i=0; i<num_cores; i++) {
			int host_code = 0;
			e_read(&mbuf, 0, 0, offset_codes[i], &host_code, 4);

		//	std::cout << "HOST: "<< i << "|" << host_code << "  ";

			int init_code = 0;
			switch (host_code) {
				case 0:
					num_done++;
					break;
				case 1:
					break;
				case 2:
					{
						if (shutdown) {
							//if shutdown in progress, set host status code to zero instead of writing data
							init_code = 0;
							e_write(&mbuf, 0, 0, offset_codes[i], &init_code, 4);
						}

						int distance = DataControl::buff.getDistance();

						if( distance > 0 ) {
							if( distance > MAX_SIZE )
								distance = MAX_SIZE;

							DataControl::buff.get(pArray, distance);

							init_code = 1;
							e_write(&mbuf, 0, 0, offset_codes[i] + 8, &distance, 4);
							e_write(&mbuf, 0, 0, offset_codes[i] + 12, pArray, sizeof(PointKey)*distance);
							e_write(&mbuf, 0, 0, offset_codes[i], &init_code, 4);
						}
						else if(DataControl::ready) {
							continue;
						}
						else {
							shutdown = true;
						}

						break;
					}
				case 3:
					//Read data here
					init_code = 2;
					e_write(&mbuf, 0, 0, offset_codes[i], &init_code, 4);
					break;
				default:
					break;

			}
		}
		//std::cout << std::endl;


		if (num_done == num_cores && !wait)
			not_done = false;
	}

	/*
	//Wait for the cores to finish processing, maybe not necessary
	int wait_counter = 0;
	while (true) {
		int num_done = 0;
		for (int i=0; i<num_cores; i++) {
			int core_code = 0;
			e_read(&mbuf, 0, 0, offset_codes[i] + 4, &core_code, 4);
			if (core_code == 0)
				num_done++;
			}

			if (num_done == num_cores)
				break;
	}*/

	std::cout << "Computation Done." << std::endl;
	std::cout << "(ERROR: " << num_errors << ")" << std::endl;
	std::cout << "STATUS-0: " << code_0 << std::endl;
	std::cout << "STATUS-1: " << code_1 << std::endl;
	std::cout << "STATUS-2: " << code_2 << std::endl;
	std::cout << "STATUS-3: " << code_3 << std::endl;
	std::cout << "STATUS-7: " << code_7 << std::endl;
	std::cout << "STATUS-8: " << code_8 << std::endl;
	std::cout << "STATUS-9: " << code_9 << std::endl;
	std::cout << "STATUS-OTHER: " << code_other << std::endl;


	for (int i=1; i<=num_cores; i++) {
		e_mem_t dbug_size;
		e_mem_t dbug_text;

		int base = 0x00100000*i;
		int text_size = 0;
		int num = 0;
		int offset = (header_size + data_size)*(i-1);

		e_alloc(&dbug_size, base, sizeof(int)*2);
		e_read(&dbug_size, 0, 0, 0, &text_size, sizeof(int));
		e_read(&dbug_size, 0, 0, 4, &num, sizeof(int));

		if (text_size == 0) {
			int status = 0;
			e_read(&mbuf, 0, 0, offset, &status, sizeof(int));
			std::cout << "No text from Core " << num << " (CODE: " << status << ")" << std::endl;
			e_free(&dbug_size);
			e_free(&dbug_text);
			continue;
		}


		e_alloc(&dbug_text, base + 8, sizeof(char)*text_size);
		char text[text_size];
		e_read(&dbug_text, 0, 0, 0, &text, sizeof(char)*text_size);
		int status_c = 0;
		e_read(&mbuf, 0, 0, offset+4, &status_c, sizeof(int));
		int status_h = 0;
		e_read(&mbuf, 0, 0, offset, &status_h, sizeof(int));

		std::cout << "//////Core " << num << " Debug//////" << std::endl;
		std::cout << "Status code host = " << status_h << std::endl;
		std::cout << "Status code core = " << status_c << std::endl;
		std::cout << "Text size=" << text_size << std::endl;

		for (int i=0; i<text_size; i++)
			std::cout << text[i];
		std::cout << std::endl;

		e_free(&dbug_size);
		e_free(&dbug_text);

	}

	e_free(&mbuf);


	std::cout << "Done running\n\n" << std::endl;

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

//Old
void DataProcessing::compute(int id) {/*
	//std::this_thread::yield();
	Log::outln(id, "Thread # starting.");

	int size = 300;
	do {
		//std::cout << "Processing 300 points\n";
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1));

		//SpinArray::DPoint * point = DataControl::buff.get();

		auto start = std::chrono::high_resolution_clock::now();
		PointKey * p = new PointKey[size];
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
*/
};

//Old
void DataProcessing::consume(int id) {
	/*
	while ( DataControl::ready) {
		//std::chrono::milliseconds(10);
		//std::cout << "Consumer " << id << "consuming.\n";
		DataControl::buff.get();
	}*/
};

void DataProcessing::fpsCounter() {

	while (DataControl::ready) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		DataControl::buff.printSize();
		std::cout << " FPS:" << DataControl::frames << std::endl;
		DataControl::frames = 0;
	//	DataControl::ready = false;

	}
}

void DataProcessing::join() {
	Log::outln("Waiting for threads...");
	thread1->join();
	fpsc->join();
	Log::outln("All threads finished.");
};
