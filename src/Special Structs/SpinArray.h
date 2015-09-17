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

        //Depth point. Contains the z value, pixel locations i and j, and the time
        /*
        struct DPoint {
            short depth;
            short i;
            short j;
        };*/

        //flag gets
        bool isOverwriting();
        bool isOverflowing();
        long getLossCount();



        //flag sets
        void setOverwrite(bool over);
        void resetCount();

        bool put(short depth, short i, short j);
        void put(short* frame, int width, int height);
        void get(Point *array, int size);
        long getDistance();
        Point* get();
        void print();
        void printSize();
        void lock();
        void unlock();

    private:
        //flags
        bool overwrite;
        bool overflow;
        long lossCounter;

        Point* top;
        long head;
        long tail;
        long length;
        std::atomic<long> distance;





};

#endif
