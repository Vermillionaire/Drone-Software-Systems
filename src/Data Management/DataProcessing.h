 #include <iostream>
 #include <thread>
 #include <unordered_map>
 #include "Point.h"


class DataProcessing {
	public:
		DataProcessing();
		~DataProcessing();

		void joinAll();


    std::unordered_map<Point,Point,PointHasher> mymap;

	private:
		std::thread *thread1;
		std::thread *thread2;
		std::thread *thread3;
		std::thread *fpsc;

		void compute(int id);
		void consume(int id);
		void inspect(int id);
		void fpsCounter();

};
