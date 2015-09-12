#include "SpinArray.h"
#include <iostream>

SpinArray::SpinArray(const int arraySize) {
    length = arraySize;

    top = new DPoint*[arraySize];
    head = 0;
    tail = 0;

    overwrite = false;
    overflow = false;
    lossCounter = 0;
};

SpinArray::~SpinArray() {
    for (int i=0; i<length; i++)
        if (top[i] != nullptr)
            delete top[i];

    delete[] top;
};

bool SpinArray::isOverwriting() {
    return overwrite;
};

bool SpinArray::isOverflowing() {
    return overflow;
};

long SpinArray::getLossCount() {
    return lossCounter;
};

void SpinArray::setOverwrite(bool over) {
    overwrite = over;
};

void SpinArray::resetCount() {
    lossCounter = 0;
};

bool SpinArray::put(DPoint* item) {
    std::lock_guard<std::mutex> lock(mutex);

    if (overflow && !overwrite) {
        lossCounter++;
        delete item;
        return false;
    }

    top[head++] = item;

    if (head == length)
        head = 0;

    if (head == tail || top[head] != nullptr)
        overflow = true;

    return true;
};


SpinArray::DPoint * SpinArray::get() {
    std::lock_guard<std::mutex> lock(mutex);

    DPoint* item = top[tail];

    if (item  != nullptr) {
        top[tail] = nullptr;

        if (tail == head);
        else if (tail+1 >= length)
            tail = 0;
        else
            tail++;
    }

    return item;
};


void SpinArray::clean() {
    std::lock_guard<std::mutex> lock(mutex);
    
    for (int i=0; i<length; i++)
        if (top[i] != nullptr)
            delete top[i];

    head = 0;
    top = 0;
    lossCounter = 0;
};

void SpinArray::print() {
    std::cout << "Head = " << head << "  Tail = " << tail << "  Loss = " << lossCounter << std::endl;
    std::cout << "FLAGS: overwrite[" << overwrite << "]  overflow[" << overflow << "]" << std::endl;
    for (int i=0; i<length; i++) {
        std::cout << " [";
        if (i == head)
            std::cout << "H";
        if (i == tail)
            std::cout << "T";
        if (top[i] != nullptr)
            std::cout << "X";

        std::cout << "] ";
    }
    std::cout << std::endl;

}

void SpinArray::printSize() {
    int size = 100;
    int scale = length / size;

    std::cout << "[";

    for (int i=0; i<size; i++) {
        int pos = i*scale;
        if (head > pos && tail < pos)
                std::cout << "X";
        if (head < pos && tail > pos)
                std::cout << "-";
        else if (pos > head && pos > tail)
                std::cout << "-";
        else if (pos < head && pos < tail) {
            if (head < tail)
                std::cout << "X";
            else
                std::cout << "-";
        }

    }

    std::cout << "] [" << length/1000 << "k] [" << head/1000 << "k] [" << tail/1000 << "k]";
}

