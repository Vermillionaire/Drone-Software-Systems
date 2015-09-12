#ifndef SPINARRAY_H
#define SPINARRAY_H

#include <thread>
#include <mutex>

class SpinArray{

    public:
        //Constructor
        SpinArray(const int arraySize);
        ~SpinArray();

        std::mutex mutex;

        //Depth point. Contains the z value, pixel locations i and j, and the time
        struct DPoint {
            short depth;
            short i;
            short j;
        };

        //flag gets
        bool isOverwriting();
        bool isOverflowing();
        long getLossCount();

        //flag sets
        void setOverwrite(bool over);
        void resetCount();

        bool put(DPoint* item);
        DPoint* get();
        void clean();
        void print();
        void printSize();

    private:
        //flags
        bool overwrite;
        bool overflow;
        long lossCounter;

        DPoint** top;
        long head;
        long tail;
        long length;





};

#endif
