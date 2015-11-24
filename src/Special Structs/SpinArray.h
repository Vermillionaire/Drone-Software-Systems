#ifndef SPINARRAY_H
#define SPINARRAY_H

#include "DataControl.h"
#include "Point.h"
#include <thread>
#include <mutex>
#include <atomic>

class SpinArray{

    public:
        //Constructor
        SpinArray(const int arraySize);
        ~SpinArray();

        std::mutex mutex;
        std::mutex other;


        //flag gets
        bool isOverwriting();
        bool isOverflowing();
        long getLossCount();



        //flag sets
        void setOverwrite(bool over);
        void resetCount();

        bool put(short depth, short i, short j);
        void put(short* frame, int width, int height);
        int get(PointKey *array, int size);
        long getDistance();
        //Point* get();
        void print();
        void printSize();
        void lock();
        void unlock();

        int getGot();
        int getPut();

    private:
        //flags
        bool overwrite;
        bool overflow;
        long lossCounter;

        PointKey* top;
        long head;
        long tail;
        long length;
        std::atomic<long> distance;

        int putVal;
        int gotVal;





};

#endif
