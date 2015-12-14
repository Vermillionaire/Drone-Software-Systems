#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <iostream>
#include <thread>
#include <unordered_map>
#include "Point.h"
#include "DataStorage.h"
#include "SpinArray.h"
#include "Log.h"
#include "e_hal.h"
#include "e_loader.h"
#include <thread>
#include <mutex>
#include <atomic>
#include "SpinWrapper.h"
#include "Constants.h"

//class SpinWrapp;

class DataProcessing {
public:
	DataProcessing();
	~DataProcessing();

	void join();
  void joinCores();
  int epiphanyInit();

  void epiphanyClose();
  void startThread();

  bool running = true;
	DataStorage store;

private:
  //e_mem_t *mbuf;
  e_epiphany_t *dev;
  int num_cores = 16;
  std::mutex mutex;
  bool end_monitor;

	std::thread *thread1;
	std::thread *thread2;
	std::thread *thread3;
  std::thread *threads;
	std::thread *fpsc;
  std::atomic<int> num_done;

//  SpinWrapper* buffer_getter;


  int getId();
	void compute(int id);
	void consume(int id);
	void fpsCounter();
  void epiphanyRun();
  void epiphanyRunPerCore();
  void epiphanyCoreMonitor();
  void epiphanyHostMonitor();

};

#endif
