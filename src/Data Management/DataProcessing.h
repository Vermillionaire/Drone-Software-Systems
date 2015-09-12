 #include <iostream>
 #include <thread>

class DataProcessing {
	public:
		DataProcessing();
		~DataProcessing();

		void joinAll();

	private:
		std::thread *thread1;
		std::thread *thread2;
		std::thread *thread3;
		std::thread *fpsc;

		struct point {
			int x;
			int y;
			int z;
		};

		void compute(int id);
		void consume(int id);
		void inspect(int id);
		void fpsCounter();

};