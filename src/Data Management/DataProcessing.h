 #include <iostream>
 #include <thread>
 #include <unordered_map>
 #include "Point.h"
 #include "DataControl.h"
 #include "DataStorage.h"
 #include "SpinArray.h"
 #include "Log.h"
 #include "e_hal.h"
 #include "e_loader.h"
 #include <thread>
 #include <mutex>
 #include <atomic>


class DataProcessing {
	public:
		DataProcessing();
		~DataProcessing();

		void join();
    void joinCores();
    int epiphanyInit();

    void epiphanyClose();
    void startThread();
    //std::unordered_map<Point,Point,PointHasher> mymap;

    const static int BASE = 0x3E000000;
    const static int CORE_BUFFER_SIZE = 0x2EE0; //12k
    //const static int NUM_CORES = 16;

	private:
    //e_mem_t *mbuf;
    e_epiphany_t *dev;
    DataStorage store;
    int num_cores = 16;
    std::mutex mutex;

		std::thread *thread1;
		std::thread *thread2;
		std::thread *thread3;
    std::thread *threads;
		std::thread *fpsc;
    std::atomic<int> num_done;


    int getId();
		void compute(int id);
		void consume(int id);
		void fpsCounter();
    void epiphanyRun();
    void epiphanyRunCore();

};
