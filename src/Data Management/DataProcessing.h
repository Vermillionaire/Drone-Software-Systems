 #include <iostream>
 #include <thread>
 #include <unordered_map>
 #include "Point.h"
 #include "DataControl.h"
 #include "SpinArray.h"
 #include "Log.h"
 #include "e_hal.h"
 #include "e_loader.h"


class DataProcessing {
	public:
		DataProcessing();
		~DataProcessing();

		void joinAll();
    void epiphanyInit();
    void startThreads();
    //std::unordered_map<Point,Point,PointHasher> mymap;

    const static int BASE = 0x3E000000;
    const static int CORE_BUFFER_SIZE = 0x2EE0; //12k
    //const static int NUM_CORES = 16;

	private:
    e_mem_t *mbuf;

		std::thread *thread1;
		std::thread *thread2;
		std::thread *thread3;
		std::thread *fpsc;

		void compute(int id);
		void consume(int id);
		void fpsCounter();

};
