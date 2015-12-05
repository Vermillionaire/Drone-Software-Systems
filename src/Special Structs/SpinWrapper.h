#ifndef SPINWRAPPER_H
#define SPINWRAPPER_H

#include <thread>
#include <mutex>
#include <atomic>
#include "SpinArray.h"

class SpinArray;

class SpinWrapper{

    public:
        SpinWrapper();
        ~SpinWrapper();

        SpinArray* getterArray();
        SpinArray* putterArray();

        SpinArray* getOne();
        SpinArray* getTwo();

    private:
      std::atomic<bool> toggle;

      SpinArray* array1;
      SpinArray* array2;
};

#endif
